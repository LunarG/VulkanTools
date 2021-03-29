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

def standalone_run(command_list, env=None):
    """This utility is used for external execution of binaries.
    It is encapsulated here and passed as a parameter to the
    higher-level execution functions because a caller (e.g.
    internal CI) may prefer to use their own execution function.
    This function always returns the generated stdout and stderr
    of the executed process so the caller can examine them."""
    p = subprocess.run(command_list, check=True, capture_output=True, env=env)
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

def checkPortabilityPresent(json_object):
    """The list of supported test cases differs when a device supports
    native portability, or whether emulated portability must be used.
    This function returns True if the passed json_object (which is
    a JSON-parsed vulkaninfo output) shows that native portability
    is present."""
    extensions = json_object.get("ArrayOfVkExtensionProperties")
    return extensions is not None and "VK_KHR_portability_subset" in [x["extensionName"] for x in extensions]


# All test cases are subclasses of BaseTest.
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

    # A subclass should override this with the proper list of input
    # files for that particular class.
    devsim_input_files = []

    # The output may either be the name of a file in the same
    # directory as this script, or an inline object that specifies
    # a subset of the JSON object that vulkaninfo outputs.
    vulkaninfo_output = None

    # A test case should override this if it needs to call vulkaninfo
    # with something other than the default arguments.
    vulkaninfo_args = ['--json']

    # These class variables are used to set up the test environment.
    # Some are not yet exercised, but are included for completeness
    # and eventual expansion.
    instance_layers = "VK_LAYER_LUNARG_device_simulation"
    debug_enable = 0
    exit_on_error = 0
    portability = 0
    extension_list = 0
    memory_flags = 0

    def __init__(self,
                 vulkaninfo_path,
                 run=standalone_run):

        self.vulkaninfo_path = vulkaninfo_path
        self.run = run

        # Create the list of absolute paths we'll need for VK_DEVSIM_FILENAME.
        devsim_absolute_files = []
        for devsim_file in self.devsim_input_files:
            full_path = os.path.abspath(devsim_file)
            devsim_absolute_files.append(full_path)
            if not os.path.isfile(full_path):
                log.debug('devsim input file %s (normalized %s) does not exist',
                          devsim_file, full_path)
        log.debug("devsim absolute files: %s", devsim_absolute_files)

        # This dictionary of environment values will be added to the run environment
        # for this test case.
        self.test_environment = {
            "VK_INSTANCE_LAYERS": str(self.instance_layers),
            "VK_DEVSIM_DEBUG_ENABLE": str(self.debug_enable),
            "VK_DEVSIM_EXIT_ON_ERROR": str(self.exit_on_error),
            "VK_DEVSIM_FILENAME": os.pathsep.join(devsim_absolute_files),
            "VK_DEVSIM_EMULATE_PORTABILITY_SUBSET_EXTENSION": str(self.portability),
            "VK_DEVSIM_MODIFY_EXTENSION_LIST": str(self.extension_list),
            "VK_DEVSIM_MODIFY_MEMORY_FLAGS": str(self.memory_flags),
        }

        # Get the expected subset dictionary output for use later.
        if isinstance(self.vulkaninfo_output, dict):
            self.expected_vulkaninfo_output = self.vulkaninfo_output
        else:
            output_file = os.path.abspath(self.vulkaninfo_output)
            if not os.path.isfile(output_file):
                raise DevsimLayerTestException("expected output file does not exist: {}".format(output_file))

            with open(output_file) as f:
                self.expected_vulkaninfo_output = json.load(f)

    # We need to be able to pass json_object for subclasses, even
    # though it is not used in the base class.
    # pylint: disable=unused-argument,no-self-use
    def checkJson(self, json_object):
        """Tests that need to also directly examine the vulkaninfo output
        will override this function to do their additional checks.  By
        default this function doesn't do any additional checking, making
        it appropriate for most of the tests."""
        return True
    # pylint: enable=unused-argument,no-self-use

    def check(self):
        """
        Base checker function that returns whether the test passed or failed.
        Compares output of the "run" function to an "answer" file.
        If the run output contains the values in the answer file, the test is considered passed.
        """
        command_list = [self.vulkaninfo_path] + self.vulkaninfo_args

        # Prepare the environment needed for running this instance.
        # We want to make sure that we don't modify the environment
        # for any of the other test instances.
        env = os.environ.copy()
        env.update(self.test_environment)

        stdout, stderr = self.run(command_list, env=env)
        log.debug("stderr from %s: %s", command_list, stderr)
        actual_vulkaninfo_output = json.loads(stdout)
        return (
            check_types_and_compare_values(self.expected_vulkaninfo_output,
                                           actual_vulkaninfo_output) and
            self.checkJson(actual_vulkaninfo_output)
        )

class DevsimTestReadMultipleInputFiles(BaseTest):
    """
    Tests devsim layer's ability to read in multiple input files and
    properly modify device queries.
    """
    devsim_input_files = [
        "devsim_test2_in1.json",
        "devsim_test2_in2.json",
        "devsim_test2_in4.json",
        "devsim_test2_in5.json"
    ]
    vulkaninfo_output = "devsim_test2_gold.json"

class PortabilityExtensionPresentEmulationOffTest(BaseTest):
    """
    Tests if the VK_KHR_portability_subset extension is available on the device without emulation.
    """
    devsim_input_files = [
        "devsim_dummy_in.json"
    ]
    vulkaninfo_output = {}

    def checkJson(self, json_object):
        return not checkPortabilityPresent(json_object)

class PortabilityExtensionPresentEmulationOnTest(BaseTest):
    """
    Tests if the VK_KHR_portability_subset extension is available on the device with
    devsim emulation.
    """
    devsim_input_files = [
        "devsim_dummy_in.json"
    ]
    vulkaninfo_output = {}
    portability = 1

    def checkJson(self, json_object):
        return checkPortabilityPresent(json_object)

class PortabilityNonEmulatedTest(BaseTest):
    """
    Tests if devsim can modify portability queries for the device when
    the VK_KHR_portability_subset extension is supported by the device.
    """
    devsim_input_files = [
        "portability_test.json"
    ]
    vulkaninfo_output = "portability_test_gold.json"
    vulkaninfo_args = ['--portability']

class PortabilityEmulatedTest(BaseTest):
    """
    Tests if devsim can modify protablity queries for the device when
    the VK_KHR_portability_subset extension is emulated by devsim.
    """
    devsim_input_files = [
        "portability_test.json"
    ]
    vulkaninfo_output = "portability_test_gold.json"
    vulkaninfo_args = ['--portability']
    portability = 1


def RunTests(vulkaninfo_path, run=standalone_run):
    """Run all appropriate test cases for the current configuration.
    The caller can pass in their own custom run() function if
    desired."""

    # We always run these devsim tests.
    test_cases = [
        DevsimTestReadMultipleInputFiles(vulkaninfo_path, run=run),
    ]

    # First check to see whether portability is present on the current
    # device.  The list of test cases will be different.
    stdout, stderr = run([vulkaninfo_path, '--json'])
    log.debug("checking if portability is present: stderr=%s", stderr)
    if checkPortabilityPresent(json.loads(stdout)):
        log.info("Testing devsim and native portability")
        test_cases.extend([
            PortabilityNonEmulatedTest(vulkaninfo_path, run=run),
            # Emulation should not override if it detects that native
            # portability is present, so the Emulated Test should also pass.
            PortabilityEmulatedTest(vulkaninfo_path, run=run),
        ])
    else:
        log.info("Testing devsim and emulated portability")
        test_cases.extend([
            PortabilityExtensionPresentEmulationOffTest(vulkaninfo_path, run=run),
            PortabilityExtensionPresentEmulationOnTest(vulkaninfo_path, run=run),
            PortabilityEmulatedTest(vulkaninfo_path, run=run),
        ])

    # Run all the established test cases.
    pass_count = 0
    fail_count = 0
    for test_case in test_cases:
        test_case_name = test_case.__class__.__name__
        log.info("RUNNING TEST %s", test_case_name)
        passed = test_case.check()
        if passed:
            log.info("PASS %s", test_case_name)
            pass_count += 1
        else:
            log.error("FAIL %s", test_case_name)
            fail_count += 1

    if fail_count == 0:
        log.info("All %d tests PASS", pass_count)
        return True

    log.error("%d test%s FAIL, %d test%s PASS",
              fail_count, "" if fail_count == 1 else "",
              pass_count, "" if pass_count == 1 else "")
    return False

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
    parser.add_argument(
        "--dir",
        help="Specify the directory to run the tests in.  Defaults to current working directory.",
        dest="directory",
        default=os.getcwd()
    )

    args = parser.parse_args()

    vulkaninfo_path = args.vulkan_tools_install_dir
    working_directory = os.path.abspath(args.directory)

    starting_working_directory = os.getcwd()
    os.chdir(working_directory)
        

    if sys.platform.startswith("win32"):
        vulkaninfo_path = os.path.join(vulkaninfo_path, "bin", "vulkaninfo.exe")
    elif sys.platform.startswith("linux"):
        vulkaninfo_path = os.path.join(vulkaninfo_path, "bin", "vulkaninfo")
    elif sys.platform.startswith("darwin"):
        vulkaninfo_path = os.path.join(vulkaninfo_path, "vulkaninfo", "vulkaninfo")

    RunTests(vulkaninfo_path)

    os.chdir(starting_working_directory)


if __name__ == '__main__':
    main()
