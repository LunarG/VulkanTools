#!/usr/bin/env python3
# Copyright (c) 2023 Valve Corporation
# Copyright (c) 2023 LunarG, Inc.

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

import argparse
import os
import sys
import shutil
import subprocess

# helper to define paths relative to the repo root
def RepoRelative(path):
    return os.path.abspath(os.path.join(os.path.dirname(__file__), '..', path))

# Points to the directory containing the top level CMakeLists.txt
PROJECT_SRC_DIR = os.path.abspath(os.path.join(os.path.split(os.path.abspath(__file__))[0], '..'))
if not os.path.isfile(f'{PROJECT_SRC_DIR}/CMakeLists.txt'):
    print(f'PROJECT_SRC_DIR invalid! {PROJECT_SRC_DIR}')
    sys.exit(1)

# Runs a command in a directory and returns its return code.
# Directory is project root by default, or a relative path from project root
def RunShellCmd(command, start_dir = PROJECT_SRC_DIR, env=None, verbose=False):
    # Flush stdout here. Helps when debugging on CI.
    sys.stdout.flush()

    if start_dir != PROJECT_SRC_DIR:
        start_dir = RepoRelative(start_dir)
    cmd_list = command.split(" ")

    if verbose:
        print(f'CICMD({cmd_list}, env={env})')
    subprocess.check_call(cmd_list, cwd=start_dir, env=env)

def main():
    configs = ['Release', 'Debug']

    parser = argparse.ArgumentParser()
    parser.add_argument('--config', type=str, choices=configs, default=configs[0])
    parser.add_argument('--app-abi', dest='android_abi', type=str, default="arm64-v8a")
    parser.add_argument('--tests', action='store_true', help='Build tests.')
    parser.add_argument('--clean', action='store_true', help='Cleans CMake build artifacts')
    args = parser.parse_args()

    cmake_config = args.config
    android_abis = args.android_abi.split(" ")
    build_tests = args.tests
    clean = args.clean

    if "ANDROID_NDK_HOME" not in os.environ:
        print("Cannot find ANDROID_NDK_HOME!")
        sys.exit(1)

    android_ndk_home = os.environ.get('ANDROID_NDK_HOME')
    android_toolchain = f'{android_ndk_home}/build/cmake/android.toolchain.cmake'

    # The only tool we require for building is CMake/Ninja
    required_cli_tools = ['cmake', 'ninja']

    print(f"ANDROID_NDK_HOME = {android_ndk_home}")
    print(f"Build configured for {cmake_config} | {android_abis}")

    if not os.path.isfile(android_toolchain):
        print(f'Unable to find android.toolchain.cmake at {android_toolchain}')
        exit(-1)

    for tool in required_cli_tools:
        path = shutil.which(tool)
        if path is None:
            print(f"Unable to find {tool}!")
            exit(-1)

        print(f"Using {tool} : {path}")

    cmake_install_dir = RepoRelative('build-android/libs')

    # Delete install directory since it could contain files from old runs
    if os.path.isdir(cmake_install_dir):
        print("Cleaning CMake install")
        shutil.rmtree(cmake_install_dir)

    for abi in android_abis:
        build_dir = RepoRelative(f'build-android/cmake/{abi}')
        lib_dir = f'lib/{abi}'

        if clean:
            print("Deleting CMakeCache.txt")

            # Delete CMakeCache.txt to ensure clean builds
            # NOTE: CMake 3.24 has --fresh which would be better to use in the future.
            cmake_cache = f'{build_dir}/CMakeCache.txt'
            if os.path.isfile(cmake_cache):
                os.remove(cmake_cache)

        cmake_cmd =  f'cmake -S . -B {build_dir} -G Ninja'

        cmake_cmd += f' -D CMAKE_BUILD_TYPE={cmake_config}'
        cmake_cmd += f' -D UPDATE_DEPS=ON -D UPDATE_DEPS_DIR={build_dir}'
        cmake_cmd += f' -D CMAKE_TOOLCHAIN_FILE={android_toolchain}'
        cmake_cmd += f' -D CMAKE_ANDROID_ARCH_ABI={abi}'
        cmake_cmd += f' -D CMAKE_INSTALL_LIBDIR={lib_dir}'
        cmake_cmd += f' -D BUILD_TESTS={build_tests}'
        cmake_cmd += f' -D CMAKE_ANDROID_STL_TYPE=c++_static'

        cmake_cmd += ' -D ANDROID_PLATFORM=29'
        cmake_cmd += ' -D ANDROID_USE_LEGACY_TOOLCHAIN_FILE=NO'

        RunShellCmd(cmake_cmd)

        build_cmd = f'cmake --build {build_dir}'
        RunShellCmd(build_cmd)

        install_cmd = f'cmake --install {build_dir} --prefix {cmake_install_dir}'

        RunShellCmd(install_cmd)

if __name__ == '__main__':
    main()