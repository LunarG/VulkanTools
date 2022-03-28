#!/usr/bin/env python3
# Copyright (c) 2021 Valve Corporation
# Copyright (c) 2021 LunarG, Inc.

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
# Author: Mark Lobodzinski <mark@lunarg.com>
# Author: Tony Barbour <tony@lunarg.com>

import os
import argparse
import shutil
import subprocess
import sys
import platform

from argparse import RawDescriptionHelpFormatter

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.split(os.path.abspath(__file__))[0], '..'))

if sys.version_info[0] != 3:
    print("This script requires Python 3. Run script with [-h] option for more details.")
    sys_exit(0)

# helper to define paths relative to the repo root
def repo_relative(path):
    return os.path.abspath(os.path.join(os.path.dirname(__file__), '..', path))

# Runs a command in a directory and returns its return code.
# Directory is project root by default, or a relative path from project root
def RunShellCmd(command, start_dir = PROJECT_ROOT):
    if start_dir != PROJECT_ROOT:
        start_dir = repo_relative(start_dir)
    cmd_list = command.split(" ")
    subprocess.check_call(cmd_list, cwd=start_dir)
 
 
# TODO: Pass this in as arg, may be useful for running locally
EXTERNAL_DIR_NAME = "external"
BUILD_DIR_NAME = "build"
INSTALL_DIR_NAME = "install"
EXTERNAL_DIR = repo_relative(EXTERNAL_DIR_NAME)
VT_BUILD_DIR = repo_relative(BUILD_DIR_NAME)
CONFIGURATIONS = ['Release', 'Debug']
DEFAULT_CONFIGURATION = CONFIGURATIONS[0]
ARCHS = [ 'x64', 'Win32' ]
DEFAULT_ARCH = ARCHS[0]
ret_code = 0
#
# Create build directory if it does not already exist
def CreateBuildDirectory(dir_path):
    if not os.path.exists(dir_path):
        os.makedirs(dir_path)

#
# Prepare the tools for testing
def BuildVT(args):
    print("Build Configuration: %s %s" % (args.arch, args.configuration))

    print("Run update_external_sources.bat")
    RunShellCmd('update_external_sources.bat')
    
    print("Run update_deps.py for VT Repository")
    update_cmd = 'python3 scripts/update_deps.py --dir %s --config %s --arch %s' % (EXTERNAL_DIR_NAME, args.configuration.lower(), args.arch.lower())
    RunShellCmd(update_cmd)

    CreateBuildDirectory(VT_BUILD_DIR)
    print("Run CMake")
    cmake_cmd = 'cmake -A %s -C ../%s/helper.cmake -DBUILD_VLF=OFF -DUSE_CCACHE=ON ..' % (args.arch, EXTERNAL_DIR_NAME)
    RunShellCmd(cmake_cmd, VT_BUILD_DIR)

    print("Build Vulkan Tools")
    os.chdir(VT_BUILD_DIR)
    build_cmd = 'cmake --build . --parallel %s' % os.cpu_count()
    RunShellCmd(build_cmd, VT_BUILD_DIR)

    print("Run Vulkan Tools Tests")
    os.chdir(VT_BUILD_DIR)
    test_cmd = 'ctest -C %s --output-on-failure --parallel %s' % (args.configuration, os.cpu_count())
    RunShellCmd(test_cmd, VT_BUILD_DIR)

#
# Module Entrypoint
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '-c', '--config', dest='configuration',
        metavar='CONFIG', action='store',
        choices=CONFIGURATIONS, default=DEFAULT_CONFIGURATION,
        help='Build target configuration. Can be one of: {0}'.format(
            ', '.join(CONFIGURATIONS)))
    parser.add_argument(
        '-a', '--arch', dest='arch',
        metavar='ARCH', action='store',
        choices=ARCHS, default=DEFAULT_ARCH)
    args = parser.parse_args()

    global ret_code
    try:
        BuildVT(args)
    except subprocess.CalledProcessError as proc_error:
        print('Command "%s" failed with return code %s' % (' '.join(proc_error.cmd), proc_error.returncode))
        sys.exit(proc_error.returncode)
    except Exception as unknown_error:
        print('An unkown error occured: %s', unknown_error)
        sys.exit(1)

    sys.exit(ret_code)

if __name__ == '__main__':
  main()
