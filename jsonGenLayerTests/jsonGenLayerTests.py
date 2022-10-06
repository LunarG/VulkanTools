# Copyright (c) 2022 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
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
# Author: Vikas Kamineni <vkamineni@nvidia.com>

# This script will run a sample vulkan application which has Json Generation
# layer enabled - JSON_GEN_LAYER_TESTS.exe. 

import os
import subprocess
import argparse
import sys
import json
import difflib
import itertools

def peek(iterable):
    try:
        first = next(iterable)
    except StopIteration:
        return None
    return first, itertools.chain([first], iterable)

def RunTest(executable_path, SHADERS_PATH):
    test_args = ['--shaders'] + [SHADERS_PATH]

    command_list = [executable_path] + test_args

    p = subprocess.run(command_list, shell=True)
    return p.stdout, p.stderr

def CompareJsonwithGolden(json_gen_path, golden_json_path):
    file_list = os.listdir(json_gen_path)
    json_file_list = []
    for each in file_list:
        if each.startswith("JSON_GEN_LAYER_TESTS") and each.endswith(".json"):
            json_file_list.append(each)

    result = True
    for each in json_file_list:
        generated_json_file = os.path.join(json_gen_path, each)
        golden_json_file = os.path.join(golden_json_path, each)

        print("==>Comparing Generated Json", each, "with Golden Json")

        with open(generated_json_file) as gen_f:
            gen_json_text = gen_f.readlines()
        with open(golden_json_file) as golden_f:
            golden_json_text = golden_f.readlines()

        difflines = difflib.unified_diff(gen_json_text, golden_json_text, fromfile=generated_json_file, tofile=golden_json_file, lineterm="")
        if peek(difflines) is None:
            print("==>Generated Json file", each, "matches with Golden Json")
        else:
            result = False
            print("==>Generated Json file", each, "does not match with Golden Json")
            print("Find the diff below - ")
            print("----------------------")
            for line in difflines:
                print(line)

        generated_json_file = ""
        golden_json_file = ""

    return result

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--release", dest="isRelease", action="store_true",
        default=False,
        help="Specify --release if using the release build of JsonGenLayerTests"
    )
    parser.add_argument(
        "json_gen_layer_install_dir",
        help="Path to the install directory of the jsonGenLayerTests repo."
    )
    parser.add_argument(
        "--shaders",
        help="Specify the path to the json_spv directory",
        dest="shaders"
    )

    args = parser.parse_args()
    json_gen_layer_install_dir = os.path.abspath(args.json_gen_layer_install_dir)
    print("Running test from: " + json_gen_layer_install_dir)
    print("Obtaining shaders from: " + args.shaders)
    SHADERS_PATH = os.path.abspath(args.shaders)

    current_directory = os.getcwd()

    buildFlavor = "Debug"
    if args.isRelease:
        buildFlavor = "Release"

    if sys.platform.startswith("win32"):
        executable_path = os.path.join(json_gen_layer_install_dir, buildFlavor, "JSON_GEN_LAYER_TESTS.exe")
    elif sys.platform.startswith("linux"):
        executable_path = os.path.join(json_gen_layer_install_dir, "JSON_GEN_LAYER_TESTS")

    print("Run JSON_GEN_LAYER_TESTS")
    print("========================")
    stdout, stderr = RunTest(executable_path, SHADERS_PATH)
    print("JSON_GEN_LAYER_TESTS ==> DONE")
    print("=============================")

    json_gen_path = os.environ["VK_JSON_FILE_PATH"]
    golden_json_path = os.path.join(current_directory, "GoldenJsonFiles")
    if CompareJsonwithGolden(json_gen_path, golden_json_path):
        print("[TEST PASSED]")
    else:
        print("[TEST FAILED]")

if __name__ == '__main__':
    main()
