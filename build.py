# =============================================================
# Copyright (c) 2018 Advanced Micro Devices, Inc.
# =============================================================

import argparse
import os
import os.path
import subprocess
import sys

if __name__ == '__main__':
    arg_parser = argparse.ArgumentParser(
        description='Vulkan Tools build script')
    arg_parser.add_argument(
        '-a', '--arch', action='store',
        default='x64',  choices=['x86', 'x64'],
        dest='arch', metavar='ARCH',
        help='Choose target architecture')
    arg_parser.add_argument(
        '-c', '--config', action='store',
        default='release',  choices=['debug', 'release'],
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
    arguments = arg_parser.parse_args(sys.argv[1:])
    if not arguments.no_deps:
        subprocess.run(['git', 'submodule', 'update', '--recursive'],
                       cwd=os.getcwd())
        subprocess.run(['update_external_sources.bat'],
                       cwd=os.getcwd())
    build_dir = 'build'
    if 'debug' == arguments.config:
        build_dir = 'dbuild'
    if 'x86' == arguments.arch:
        build_dir = build_dir + '32'
    build_path = os.path.join(os.getcwd(), build_dir)
    os.makedirs(build_path, mode=0o744, exist_ok=True)
    subprocess.run([
        'python', os.path.join('..', 'scripts', 'update_deps.py'),
        '--config', arguments.config.capitalize(),
        '--arch', arguments.arch],
        cwd=build_path)
    subprocess.run(
        ['cmake ',
         '-A', arguments.arch, '--config', arguments.config.capitalize(),
         '-C', 'helper.cmake', os.getcwd()],
        cwd=build_path)
    cmake_build_args = ['cmake', '--build', '.',
                        '--config', arguments.config.capitalize()]
    if arguments.install:
        cmake_build_args.extend(['--target', 'install'])
    subprocess.run(cmake_build_args, cwd=build_path)
