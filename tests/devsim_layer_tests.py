#!/usr/bin/env python3
# Copyright (c) 2020-2021 LunarG, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Author: Jeremy Kniager <jeremyk@lunarg.com>
"""
Some objects and functions to run tests for the VK_LAYER_LUNARG_device_simulation layer.
This script will be modified in the future when LunarG/VulkanTools is updated to use the
CI test harness written by Bob Ellison.  It is intended to be usable by VulkanTools
developers in their own repository clones, as well as in external CI using GitHub Actions
and internal CI using Jenkins.

Only desktop Vulkan is currently supported; considerable redesign would be
required to be able to do these exercises on Android.
"""

import argparse
import json
import logging
import os
import subprocess
import sys

log = logging.getLogger(__name__)

def standalone_run(command_list):
    """This utility is used for external execution of binaries.
    It is encapsulated here and passed as a parameter to the
    higher-level execution functions because a caller (e.g.
    internal CI) may prefer to use their own execution function.
    This function always returns the generated stdout and stderr
    of the executed process so the caller can examine them."""
    p = subprocess.run(command_list, check=True, capture_output=True)
    return p.stdout, p.stderr

# Our tests will have vulkaninfo output with some expected values and
# a lot of values that we don't care about (and that are sensitive to the test
# environment, like the SDK version or the graphics driver version), e.g.
# a list of all extensions... When we compare our generated output with
# expected output, we really just want to make sure the values we need
# are present, and ignore everything else.
#
# We could implement this with a custom dict class that alters the
# comparison operator.  But for now we're implementing our own recursive
# dictionary comparisons and list comparisons (because we likely have lists
# of dictionaries).

def compare_lists(list_1, list_2, debug_indent=""):
    """
    A custom method for comparing lists.
    """
    log.debug('%sComparing lists...', debug_indent)

    if len(list_1) != len(list_2):
        log.debug("%sLists unequal lengths! %d vs %d", debug_indent, len(list_1), len(list_2))
        return False
    for value_1, value_2 in zip(list_1, list_2):
        if not check_types_and_compare_values(value_1,
                                              value_2,
                                              debug_indent + "\t"):
            return False

    return True


def is_dict_subset(dict_1, dict_2, debug_indent=""):
    """
    A custom method for comparing dictionaries.
    Dictionaries do not have to be completely equal.
    dict_1 simply needs to be a subset contained within dict_2.
    """
    log.debug("%sComparing dictionaries...", debug_indent)

    for k in dict_1:
        try:
            if not check_types_and_compare_values(dict_1[k],
                                                  dict_2[k],
                                                  debug_indent + "\t"):
                return False
        except KeyError:
            return False

    return True


def check_types_and_compare_values(struct_1, struct_2, debug_indent=""):
    """
    Does some error checking before passing the structs onto appropriate compare functions.
    """

    if isinstance(struct_1, list) and isinstance(struct_2, list):
        return compare_lists(struct_1, struct_2, debug_indent)

    if isinstance(struct_1, dict) and isinstance(struct_2, dict):
        return is_dict_subset(struct_1, struct_2, debug_indent)

    if struct_1 != struct_2:
        log.debug("%sPrimitive comparison of %s and %s failed!", debug_indent, struct_1, struct_2)
        return False

    return True

class DevsimLayerTestException(Exception):
    """The exception class for this module."""

class BaseTest:
    """
    A class that implements and defines some base functions and a base initializer for devsim tests.
    The devsim_input_files are a list of filenames; if they're relative, they're relative to
    the same directory where this script resides.  Absolute paths may also be provided.
    The expected_vulkaninfo_output can be a dictionary containing subset vulkaninfo
    output to compare against, or can specify a relative or absolute filename that contains
    a more extensive set of vulkaninfo output.

    This test requires that a valid VK_LAYER_PATH and (for Linux) LD_LIBRARY_PATH
    are already set up in the enviroment.
    """
    def __init__(self,
                 devsim_input_files,
                 expected_vulkaninfo_output,
                 vulkaninfo_path,
                 vulkaninfo_args=None,
                 run=standalone_run,
                 # These variables specify the environment variables needed
                 # for a particular test case.
                 instance_layers="VK_LAYER_LUNARG_device_simulation",
                 debug_enable=0,
                 exit_on_error=0,
                 filename="",
                 portability=0,
                 extension_list=0):

        self.vulkaninfo_path = vulkaninfo_path
        self.run = run

        # Create the list of absolute paths we'll need for VK_DEVSIM_FILENAME.
        devsim_absolute_files = []
        for devsim_file in devsim_input_files:
            full_path = os.path.abspath(devsim_file)
            devsim_absolute_files.append(full_path)
            if not os.path.isfile(full_path):
                log.debug('devsim input file %s (normalized %s) does not exist',
                          devsim_file, full_path)
        log.debug("devsim absolute files: %s", devsim_absolute_files)

        # This dictionary of environment values will be added to the run environment
        # for this test case.
        self.test_environment = {
            "VK_INSTANCE_LAYERS": str(instance_layers),
            "VK_DEVSIM_DEBUG_ENABLE": str(debug_enable),
            "VK_DEVSIM_EXIT_ON_ERROR": str(exit_on_error),
            "VK_DEVSIM_FILENAME": os.pathsep.join(devsim_absolute_files),
            "VK_DEVSIM_EMULATE_PORTABILITY_SUBSET_EXTENSION": str(portability),
            "VK_DEVSIM_MODIFY_EXTENSION_LIST": str(extension_list),
        }

        # Get the expected subset dictionary output for use later.
        if isinstance(expected_vulkaninfo_output, dict):
            self.expected_vulkaninfo_output = expected_vulkaninfo_output
        else:
            output_file = os.path.abspath(expected_vulkaninfo_output)
            if not os.path.isfile(output_file):
                raise DevsimLayerTestException("expected output file does not exist: {}".format(output_file))

            with open(output_file) as f:
                self.expected_vulkaninfo_output = json.load(f)

    def check_output(self, stdout):
        """Tests that need to also directly examine the vulkaninfo output
        will override this function to do their additional checks.  By
        default this function doesn't do any additional checking, making
        it appropriate for most of the tests."""
        return True

    def check(self):
        """
        Base checker function that returns whether the test passed or failed.
        Compares output of the "run" function to an "answer" file.
        If the run output contains the values in the answer file, the test is considered passed.
        """
        command_list = [self.vulkaninfo_path]
        if self.vulkaninfo_args is not None:
            command_list.extend(self.vulkaninfo_args)
        stdout, stderr = self.run(command_list)
        actual_vulkaninfo_output = json.loads(stdout)
        return (
            check_types_and_compare_values(self.expected_vulkaninfo_output,
                                           actual_vulkaninfo_output) and
            self.check(stdout)
        )

class BasicTest(BaseTest):
    """
    A basic test that runs vulkaninfo with devsim and a given config file,
    recording the output to a JSON file.
    """
    def run(self, vulkaninfo_path):
        self.test_environment.setupEnvironment()
        with open(self.output_filepath, 'w') as output:
            subprocess.run([vulkaninfo_path, '--json'], check=True, stdout=output)


class PortabilityTest(BaseTest):
    """
    A test that runs vulkaninfo and devsim with portability on,
    recording the output to a JSON file.
    """
    def run(self, vulkaninfo_path):
        self.test_environment.setupEnvironment()
        with open(self.output_filepath, 'w') as output:
            subprocess.run([vulkaninfo_path, '--portability'], check=True, stdout=output)


class PortabilityExtensionPresentTest(BasicTest):
    """
    A test that modifies the "check" function to pass if
    the VK_KHR_portability_subset extension is present on the device or added by devsim.
    """
    def check(self):
        with open(self.output_filepath) as output_file:
            test_results = json.load(output_file)
            extensions = test_results.get("ArrayOfVkExtensionProperties")
            return extensions is not None and "VK_KHR_portability_subset" in [x["extensionName"] for x in extensions]


class TestEnvironmentSettings:
    """
    This class holds a dictionary of environment variables and what to set them too.
    It acts as a sort of staging so that the script can set up
    curtom environment settings necessary for each test.
    """
    def __init__(self,
                 instance_layers="VK_LAYER_LUNARG_device_simulation",
                 debug_enable=0,
                 exit_on_error=0,
                 filename="",
                 portability=0,
                 extension_list=0):
        self.settings = {
            "VK_INSTANCE_LAYERS": instance_layers,
            "VK_DEVSIM_DEBUG_ENABLE": debug_enable,
            "VK_DEVSIM_EXIT_ON_ERROR": exit_on_error,
            "VK_DEVSIM_FILENAME": filename,
            "VK_DEVSIM_EMULATE_PORTABILITY_SUBSET_EXTENSION": portability,
            "VK_DEVSIM_MODIFY_EXTENSION_LIST": extension_list
        }

    def setEnvironmentSetting(self, environment_variable, value):
        """
        Insert or change a setting's value in the self.settings.
        Note: This does not immediately change the environment variable.
        """
        if VERBOSE:
            print("SETTING {}={}".format(environment_variable, value))
        self.settings[environment_variable] = value

    def setupEnvironment(self):
        """
        Sets the system environment variables to the values stored in self.settings.
        """
        for k, v in self.settings.items():
            if VERBOSE:
                print("{}={}".format(k, str(v)))
            os.environ[k] = str(v)

    def __str__(self):
        out = ""
        for k in list(self.settings.keys()):
            out += "{}={}\n".format(k, self.settings[k])

        return out


class TestFramework:
    """
    A wrapper class for tests to streamline setting up and running them.
    """
    def __init__(self, in_files, test_comp, vt_install_dir):
        self.test_comp = test_comp

        self.seperator = ':'
        if sys.platform.startswith("win32"):
            self.seperator = ';'
        self.in_files = ""
        self.setInputFiles(in_files)
        self.vt_install_dir = vt_install_dir

    def setInputFiles(self, in_files):
        """
        Sets the VK_DEVSIM_FILENAME environment variable to
        the list of config files used for the test.
        """
        self.in_files = ""
        for in_f in in_files:
            self.in_files += "{}{}".format(os.path.join(os.getcwd(), in_f), self.seperator)
        self.in_files = self.in_files[:-1]

        self.test_comp.test_environment.setEnvironmentSetting("VK_DEVSIM_FILENAME", self.in_files)

    def runAndCheck(self):
        """
        Runs self.test_comp, and returns if it passed or failed.
        """
        self.test_comp.run(self.vt_install_dir)
        return self.test_comp.check()


class DevsimTestReadMultipleInputFiles(TestFramework):
    """
    Tests devsim layer's ability to read in multiple input files and
    properly modify device queries.
    """
    test_type = BasicTest(
                          TestEnvironmentSettings(),
                          "test1_vulkaninfo_out.json",
                          "devsim_test2_gold.json"
                         )
    test_in_files = [
                     "devsim_test2_in1.json",
                     "devsim_test2_in2.json",
                     "devsim_test2_in3.json",
                     "devsim_test2_in4.json",
                     "devsim_test2_in5.json"
                    ]

    def __init__(self, vulkaninfo_path):
        TestFramework.__init__(self, self.test_in_files, self.test_type, vulkaninfo_path)


class PortabilityExtensionPresentEmulationOffTest(TestFramework):
    """
    Tests if the VK_KHR_portability_subset extension is available on the device without emulation.
    """
    test_type = PortabilityExtensionPresentTest(
                                                TestEnvironmentSettings(),
                                                "port_extent_test_vulkaninfo_out.json",
                                                None
                                               )
    test_in_files = [
                     "devsim_dummy_in.json"
                    ]

    def __init__(self, vulkaninfo_path):
        TestFramework.__init__(self, self.test_in_files, self.test_type, vulkaninfo_path)


class PortabilityExtensionPresentEmulationOnTest(TestFramework):
    """
    Tests if the VK_KHR_portability_subset extension is available on the device with
    devsim emulation.
    """
    test_type = PortabilityExtensionPresentTest(
                                                TestEnvironmentSettings(portability=1),
                                                "port_extent_test_vulkaninfo_out.json",
                                                None
                                               )
    test_in_files = [
                     "devsim_dummy_in.json"
                    ]

    def __init__(self, vulkaninfo_path):
        TestFramework.__init__(self, self.test_in_files, self.test_type, vulkaninfo_path)


class PortabilityNonEmulatedTest(TestFramework):
    """
    Tests if devsim can modify protablity queries for the device when
    the VK_KHR_portability_subset extension is supported by the device.
    """
    test_type = PortabilityTest(
                                TestEnvironmentSettings(),
                                "port_test_vulkaninfo_out.json",
                                "portability_test_gold.json"
                               )
    test_in_files = [
                     "portability_test.json"
                    ]

    def __init__(self, vulkaninfo_path):
        TestFramework.__init__(self, self.test_in_files, self.test_type, vulkaninfo_path)


class PortabilityEmulatedTest(TestFramework):
    """
    Tests if devsim can modify protablity queries for the device when
    the VK_KHR_portability_subset extension is emulated by devsim.
    """
    test_type = PortabilityTest(
                                TestEnvironmentSettings(portability=1),
                                "port_test_vulkaninfo_out.json",
                                "portability_test_gold.json"
                               )
    test_in_files = [
                     "portability_test.json"
                    ]

    def __init__(self, vulkaninfo_path):
        TestFramework.__init__(self, self.test_in_files, self.test_type, vulkaninfo_path)


def main():
    """
    Main function to run the basic tests.
    """

    logging.basicConfig(level=logging.INFO)

    parser = argparse.ArgumentParser()
    parser.add_argument(
        "vulkan_tools_install_dir",
        help="Path to the install directory of the KhronosGroup/Vulkan-Tools repo."
    )

    args = parser.parse_args()

    vulkaninfo_path = args.vulkan_tools_install_dir

    if sys.platform.startswith("win32"):
        vulkaninfo_path = os.path.join(vulkaninfo_path, "bin", "vulkaninfo.exe")
    elif sys.platform.startswith("linux"):
        vulkaninfo_path = os.path.join(vulkaninfo_path, "bin", "vulkaninfo")
    elif sys.platform.startswith("darwin"):
        vulkaninfo_path = os.path.join(vulkaninfo_path, "vulkaninfo", "vulkaninfo")

    test_dict = {
        "DevsimTestReadMultipleInputFiles":
            DevsimTestReadMultipleInputFiles(vulkaninfo_path),
        "PortabilityExtensionPresentEmulationOffTest":
            PortabilityExtensionPresentEmulationOffTest(vulkaninfo_path),
        "PortabilityExtensionPresentEmulationOnTest":
            PortabilityExtensionPresentEmulationOnTest(vulkaninfo_path),
        "PortabilityNonEmulatedTest":
            PortabilityNonEmulatedTest(vulkaninfo_path),
        "PortabilityEmulatedTest":
            PortabilityEmulatedTest(vulkaninfo_path),
    }

    test_blacklist = []

    if not test_dict["PortabilityExtensionPresentEmulationOffTest"].runAndCheck():
        test_blacklist.append("PortabilityExtensionPresentEmulationOffTest")
        test_blacklist.append("PortabilityNonEmulatedTest")

    for k in test_dict:
        if k in test_blacklist:
            continue
        print("RUNNING TEST", k)
        print("PASS" if test_dict[k].runAndCheck() else "FAIL")


if __name__ == '__main__':
    main()
