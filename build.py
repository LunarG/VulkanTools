# =============================================================
# Copyright (c) 2018 Advanced Micro Devices, Inc.
# =============================================================

import argparse
import os
import os.path
import platform
import subprocess
import sys

ANDROID_ARCH_BUILD_ERROR_MSG = '''
Error: Android builds are not supported by this script.
       You can manually build for Android by following the instruction in the
       BUILD.md file.
'''

if __name__ == '__main__':
    arg_parser = argparse.ArgumentParser(
        description='Vulkan Tools build script')
    arg_parser.add_argument(
        '--build-dir', action='store',
        default=None,
        dest='build_dir', metavar='BUILD_DIR',
        help="Choose build directory, if omitted will use [d]build[32]")
    arg_parser.add_argument(
        '-a', '--arch', action='store',
        default='x64',  choices=['x86', 'x64', 'android'],
        dest='arch', metavar='ARCH',
        help='Choose target architecture')
    arg_parser.add_argument(
        '-c', '--config', action='store',
        default='release',  choices=['Debug', 'debug', 'Release', 'release'],
        dest='config', metavar='CONFIG',
        help='Choose target build configuration')
    arg_parser.add_argument(
        '-i', '--install', action='store_true',
        default=False,
        dest='install',
        help='Build install target')
    arg_parser.add_argument(
        '--no-deps', action='store_true',
        default=False,
        dest='no_deps',
        help='Do not update external dependencies')
    arg_parser.add_argument(
        '--no-generator', action='store_true',
        default=False,
        dest='no_generator',
        help='Do not run cmake build file generator')
    arg_parser.add_argument(
        '--no-build', action='store_true',
        default=False,
        dest='no_build',
        help='Do not run cmake build, only generate build files')
    arguments = arg_parser.parse_args(sys.argv[1:])
    if 'android' == arguments.arch:
        print(ANDROID_ARCH_BUILD_ERROR_MSG)
        exit(1)
    if not arguments.no_deps:
        subprocess.run(['git', 'submodule', 'update', '--recursive'],
                       cwd=os.getcwd())
        if 'windows' == platform.system().lower():
            subprocess.run(['update_external_sources.bat'],
                           cwd=os.getcwd())
        else:
            subprocess.run(['update_external_sources.sh'],
                           cwd=os.getcwd())
    build_dir = arguments.build_dir
    if build_dir is None:
        if 'debug' == arguments.config.lower():
            build_dir = 'dbuild'
        else:
            build_dir = 'build'
        if 'x86' == arguments.arch:
            build_dir = build_dir + '32'
    build_path = os.path.join(os.getcwd(), build_dir)
    os.makedirs(build_path, mode=0o744, exist_ok=True)
    subprocess.run([
        'python', os.path.join(os.path.split(os.path.abspath(__file__))[0],
                               'scripts', 'update_deps.py'),
        '--config', arguments.config.capitalize(),
        '--arch', arguments.arch],
        cwd=build_path)
    if not arguments.no_generator:
        subprocess.run(
            ['cmake ',
             '-A', arguments.arch, '--config', arguments.config.capitalize(),
             '-C', 'helper.cmake', os.getcwd()],
            cwd=build_path)
    if not arguments.no_build:
        cmake_build_args = ['cmake', '--build', '.',
                            '--config', arguments.config.capitalize()]
        if arguments.install:
            cmake_build_args.extend(['--target', 'install'])
        subprocess.run(cmake_build_args, cwd=build_path)
