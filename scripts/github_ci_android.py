#!/usr/bin/env python3
# Copyright (c) 2021Valve Corporation
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
 
SUPPORTED_ABIS = [ 'arm64-v8a', 'armeabi-v7a']
DEFAULT_ABI = SUPPORTED_ABIS[0]
#
# Fetch Android components, build Android VVL
def BuildAndroid(target_abi):
    print("Fetching NDK\n")
    wget_cmd = 'wget http://dl.google.com/android/repository/android-ndk-r21d-linux-x86_64.zip'
    RunShellCmd(wget_cmd)

    print("Extracting NDK components\n")
    unzip_cmd = 'unzip -u -q android-ndk-r21d-linux-x86_64.zip'
    RunShellCmd(unzip_cmd)
    # Add NDK to path
    os.environ['ANDROID_NDK_HOME'] = repo_relative('android-ndk-r21d')
    os.environ['PATH'] = os.environ.get('ANDROID_NDK_HOME') + os.pathsep + os.environ.get('PATH')

    print("Preparing Android Dependencies\n")
    update_sources_cmd = './update_external_sources_android.sh --abi %s --no-build' % target_abi
    RunShellCmd(update_sources_cmd, "build-android")
    
    print("Generating\n")
    generate_cmd = './android-generate.sh'
    RunShellCmd(generate_cmd, "build-android") 

    print("Building Android Layers and Tests\n")
    ndk_build_cmd = 'ndk-build APP_ABI=%s -j%s' % (target_abi, os.cpu_count())
    RunShellCmd(ndk_build_cmd, "build-android")
    
#
# Module Entrypoint
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '-a', '--abi', dest='target_abi',
        metavar='ABI', action='store',
        choices=SUPPORTED_ABIS, default=DEFAULT_ABI,
        help='Build target ABI. Can be one of: {0}'.format(
            ', '.join(SUPPORTED_ABIS)))
    args = parser.parse_args()

    try:
       BuildAndroid(args.target_abi)

    except subprocess.CalledProcessError as proc_error:
        print('Command "%s" failed with return code %s' % (' '.join(proc_error.cmd), proc_error.returncode))
        sys.exit(proc_error.returncode)
    except Exception as unknown_error:
        print('An unkown error occured: %s', unknown_error)
        sys.exit(1)

    sys.exit(0)

if __name__ == '__main__':
  main()
