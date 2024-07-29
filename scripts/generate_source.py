#!/usr/bin/env python3
# Copyright (c) 2021-2024 The Khronos Group Inc.
# Copyright (c) 2021-2024 Valve Corporation
# Copyright (c) 2021-2024 LunarG, Inc.
# Copyright (c) 2021-2024 Google Inc.
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

import argparse
import filecmp
import os
import shutil
import subprocess
import sys
import tempfile
import difflib
import json

# Helper to define paths relative to the repo root
def repo_relative(path):
    return os.path.abspath(os.path.join(os.path.dirname(__file__), '..', path))

# Files to exclude from --verify check
verify_exclude = ['.clang-format']

def main(argv):
    parser = argparse.ArgumentParser(description='Generate source code for this repository')
    parser.add_argument('registry', metavar='REGISTRY_PATH', help='path to the Vulkan-Headers registry directory')
    parser.add_argument('--generated-version', help='sets the header version used to generate the repo')
    group = parser.add_mutually_exclusive_group()
    group.add_argument('-i', '--incremental', action='store_true', help='only update repo files that change')
    group.add_argument('-v', '--verify', action='store_true', help='verify repo files match generator output')
    args = parser.parse_args(argv)

    gen_spec_cmds = [*[[repo_relative('scripts/vt_genvk.py'),
                   '-registry', os.path.abspath(os.path.join(args.registry, 'vk.xml')),
                   '-scripts', os.path.abspath(args.registry),
                   '-quiet',
                   filename] for filename in ["api_dump.cpp",
                                              "api_dump_text.h",
                                              "api_dump_html.h",
                                              "api_dump_json.h",
                                              "api_dump_text.cpp",
                                              "api_dump_html.cpp",
                                              "api_dump_json.cpp"]]]

    gen_video_cmds = [*[[repo_relative('scripts/vt_genvk.py'),
                   '-registry', os.path.abspath(os.path.join(args.registry, 'video.xml')),
                   '-scripts', os.path.abspath(args.registry),
                   '-quiet',
                   filename] for filename in ["api_dump_video_text.h",
                                              "api_dump_video_html.h",
                                              "api_dump_video_json.h"]]]

    gen_cmds = gen_spec_cmds + gen_video_cmds

    repo_dir = repo_relative('layersvt/generated')

    # Update the api_version in the respective json files
    if args.generated_version:
        json_files = []
        json_files.append(repo_relative('layersvt/json/VkLayer_api_dump.json.in'))
        for json_file in json_files:
            with open(json_file) as f:
                data = json.load(f)

            data["layer"]["api_version"] = args.generated_version

            with open(json_file, mode='w', encoding='utf-8', newline='\n') as f:
                f.write(json.dumps(data, indent=4))

    # Get directory where generators will run
    if args.verify or args.incremental:
        # Generate in temp directory so we can compare or copy later
        temp_obj = tempfile.TemporaryDirectory(prefix='vt_codegen_')
        temp_dir = temp_obj.name
        gen_dir = temp_dir
    else:
        # Generate directly in the repo
        gen_dir = repo_dir

    # Run each code generator
    for cmd in gen_cmds:
        print(' '.join(cmd))
        try:
            subprocess.check_call([sys.executable] + cmd, cwd=gen_dir)
        except Exception as e:
            print('ERROR:', str(e))
            return 1

    # Optional post-generation steps
    if args.verify:
        # Compare contents of temp dir and repo
        temp_files = set(os.listdir(temp_dir))
        repo_files = set(os.listdir(repo_dir))
        files_match = True
        for filename in sorted((temp_files | repo_files) - set(verify_exclude)):
            temp_filename = os.path.join(temp_dir, filename)
            repo_filename = os.path.join(repo_dir, filename)
            if filename not in repo_files:
                print('ERROR: Missing repo file', filename)
                files_match = False
            elif filename not in temp_files:
                print('ERROR: Missing generator for', filename)
                files_match = False
            elif not filecmp.cmp(temp_filename, repo_filename, shallow=False):
                print('ERROR: Repo files do not match generator output for', filename)
                files_match = False
                # print line diff on file mismatch
                with open(temp_filename) as temp_file, open(repo_filename) as repo_file:
                    print(''.join(difflib.unified_diff(temp_file.readlines(),
                                                       repo_file.readlines(),
                                                       fromfile='temp/' + filename,
                                                       tofile=  'repo/' + filename)))

        # Return code for test scripts
        if files_match:
            print('SUCCESS: Repo files match generator output')
            return 0
        return 1

    elif args.incremental:
        # Copy missing or differing files from temp directory to repo
        for filename in os.listdir(temp_dir):
            temp_filename = os.path.join(temp_dir, filename)
            repo_filename = os.path.join(repo_dir, filename)
            if not os.path.exists(repo_filename) or \
               not filecmp.cmp(temp_filename, repo_filename, shallow=False):
                print('update', repo_filename)
                shutil.copyfile(temp_filename, repo_filename)

    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))

