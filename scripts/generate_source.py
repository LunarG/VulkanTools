#!/usr/bin/env python3
# Copyright (c) 2021-2025 The Khronos Group Inc.
# Copyright (c) 2021-2025 Valve Corporation
# Copyright (c) 2021-2025 LunarG, Inc.
# Copyright (c) 2021-2024 Google Inc.
# Copyright (c) 2023-2024 RasterGrid Kft.
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
import sys
import tempfile
import difflib
import json
import pickle
import common_codegen
from xml.etree import ElementTree

def RunGenerators(api: str, registry: str, video_registry: str, directory: str, styleFile: str, targetFilter: str, caching: bool):

    try:
        code = common_codegen.RunShellCmd('clang-format --version')
        has_clang_format = True
    except:
        has_clang_format = False

    if not has_clang_format:
        print("WARNING: Unable to find clang-format!")

    # These live in the Vulkan-Docs repo, but are pulled in via the
    # Vulkan-Headers/registry folder
    # At runtime we inject python path to find these helper scripts
    scripts = os.path.dirname(registry)
    scripts_directory_path = os.path.dirname(os.path.abspath(__file__))
    registry_headers_path = os.path.join(scripts_directory_path, scripts)
    sys.path.append(registry_headers_path)
    try:
        from reg import Registry
    except:
        print("ModuleNotFoundError: No module named 'reg'") # normal python error message
        print(f'{registry_headers_path} is not pointing to the Vulkan-Headers registry directory.')
        print("Inside Vulkan-Headers there is a registry/reg.py file that is used.")
        sys.exit(1) # Return without call stack so easy to spot error

    from base_generator import BaseGeneratorOptions
    from generators.api_dump_generator import ApiDumpGenerator

    # These set fields that are needed by both OutputGenerator and BaseGenerator,
    # but are uniform and don't need to be set at a per-generated file level
    from base_generator import SetOutputDirectory, SetTargetApiName, SetMergedApiNames, EnableCaching
    SetOutputDirectory(directory)
    SetTargetApiName(api)

    # Build up a list of all generators
    # Note: Options variable names MUST match order of constructor variable in generator
    generators = {
        'api_dump_dispatch.cpp' : {
            'generator' : ApiDumpGenerator,
            'genCombined': True,
            'registry' : 'vk',
        },
        'api_dump_implementation.h' : {
            'generator' : ApiDumpGenerator,
            'genCombined': True,
            'registry' : 'vk',
        },
        'api_dump_video_implementation.h' : {
            'generator' : ApiDumpGenerator,
            'genCombined': True,
            'registry' : 'video',
        },
    }

    unknownTargets = [x for x in (targetFilter if targetFilter else []) if x not in generators.keys()]
    if unknownTargets:
        print(f'ERROR: No generator options for unknown target(s): {", ".join(unknownTargets)}', file=sys.stderr)
        return 1

    # Filter if --target is passed in
    targets = [x for x in generators.keys() if not targetFilter or x in targetFilter]

    cachePath = os.path.join(tempfile.gettempdir(), f'vkobject_{os.getpid()}')
    cacheData = None
    cacheType = None

    if caching:
        EnableCaching()

    for index, target in enumerate(targets, start=1):
        print(f'[{index}|{len(targets)}] Generating {target}')

        # First grab a class contructor object and create an instance
        targetGenerator = generators[target]['generator']
        generatorOptions = generators[target]['options'] if 'options' in generators[target] else []
        generator = targetGenerator(*generatorOptions)

        # This code and the 'genCombined' generator metadata is used by downstream
        # users to generate code with all Vulkan APIs merged into the target API variant
        # (e.g. Vulkan SC) when needed. The constructed apiList is also used to filter
        # out non-applicable extensions later below.
        apiList = [api]
        if api != 'vulkan' and generators[target]['genCombined']:
            SetMergedApiNames('vulkan')
            apiList.append('vulkan')
        else:
            SetMergedApiNames(None)

        baseOptions = BaseGeneratorOptions(customFileName = target)

        # Create the registry object with the specified generator and generator
        # options. The options are set before XML loading as they may affect it.
        reg = Registry(generator, baseOptions)

        # Parse the specified registry XML into an ElementTree object
        if generators[target]['registry'] == 'vk':
            tree = ElementTree.parse(registry)

        elif generators[target]['registry'] == 'video':
            tree = ElementTree.parse(video_registry)
        else:
            print(f'{target} didn\'t specify the registry to use')
            return -1

        # Filter out extensions that are not on the API list
        [exts.remove(e) for exts in tree.findall('extensions') for e in exts.findall('extension') if (sup := e.get('supported')) is not None and all(api not in sup.split(',') for api in apiList)]

        # Load the XML tree into the registry object
        reg.loadElementTree(tree)

        # The cached data is saved inside the BaseGenerator, so search for it and try
        # to reuse the parsing for each generator file.
        if caching:
            # Old cache is stale, delete it since it doesn't match the currently cached registry
            if os.path.isfile(cachePath) and cacheType is not None and cacheType == generators[target]['registry']:
                os.remove(cachePath)
                cacheData = None
            elif not cacheData and os.path.isfile(cachePath):
                file = open(cachePath, 'rb')
                cacheData = pickle.load(file)
                file.close()


        if caching and (cacheData) and ('regenerate' not in generators[target] or not generators[target]['regenerate']) and cacheType == generators[target]['registry']:
            # TODO - We shouldn't have to regenerate any files, need to investigate why we some scripts need it
            reg.gen.generateFromCache(cacheData, reg.genOpts)
        else:
            # Finally, use the output generator to create the requested target
            reg.apiGen()

        if caching:
            cacheType = generators[target]['registry']

        # Run clang-format on the file
        if has_clang_format:
            common_codegen.RunShellCmd(f'clang-format -i --style=file:{styleFile} {os.path.join(directory, target)}')

    if os.path.isfile(cachePath):
        os.remove(cachePath)

# helper to define paths relative to the repo root
def repo_relative(path):
    return os.path.abspath(os.path.join(os.path.dirname(__file__), '..', path))

def main(argv):
    # files to exclude from --verify check
    verify_exclude = [
        '.clang-format'
    ]

    parser = argparse.ArgumentParser(description='Generate source code for this repository')
    parser.add_argument('--api',
                        default='vulkan',
                        choices=['vulkan'],
                        help='Specify API name to generate')
    parser.add_argument('registry', metavar='REGISTRY_PATH', help='path to the Vulkan-Headers registry directory')
    parser.add_argument('--generated-version', help='sets the header version used to generate the repo')
    parser.add_argument('-o', help='Create target and related files in specified directory.', dest='output_directory')
    group = parser.add_mutually_exclusive_group()
    group.add_argument('--target', nargs='+', help='only generate file names passed in')
    group.add_argument('-i', '--incremental', action='store_true', help='only update repo files that change')
    group.add_argument('-v', '--verify', action='store_true', help='verify repo files match generator output')
    group.add_argument('--no-caching', action='store_true', help='Do not try to cache generator objects')
    args = parser.parse_args(argv)

    out_dir = repo_relative('layersvt/generated')

    # Need pass style file incase running with --verify and it can't find the file automatically in the temp directory
    styleFile = os.path.join(out_dir, '.clang-format')

    # Update the api_version in the respective json files
    if args.generated_version:
        json_files = []
        json_files.append(repo_relative('layersvt/json/VkLayer_api_dump.json.in'))
        for json_file in json_files:
            with open(json_file, mode='r') as f:
                data = json.load(f)

            data["layer"]["api_version"] = args.generated_version

            with open(json_file, mode='w', encoding='utf-8', newline='\n') as f:
                f.write(json.dumps(data, indent=4))

    # get directory where generators will run
    if args.verify or args.incremental:
        # generate in temp directory so we can compare or copy later
        temp_obj = tempfile.TemporaryDirectory(prefix='vulkan_tools_codegen_')
        temp_dir = temp_obj.name
        gen_dir = temp_dir
    else:
        # generate directly in the repo
        gen_dir = out_dir

    if args.output_directory is not None:
        gen_dir = args.output_directory

    registry = os.path.abspath(os.path.join(args.registry,  'vk.xml'))
    if not os.path.isfile(registry):
        print(f'{registry} does not exist')
        return -1
    video_registry = os.path.abspath(os.path.join(args.registry,  'video.xml'))
    if not os.path.isfile(video_registry):
        print(f'{video_registry} does not exist')
        return -1

    caching = not args.no_caching
    RunGenerators(args.api, registry, video_registry, gen_dir, styleFile, args.target, caching)

    # optional post-generation steps
    if args.verify:
        # compare contents of temp dir and repo
        temp_files = set(os.listdir(temp_dir))
        repo_files = set(os.listdir(out_dir))
        for filename in sorted((temp_files | repo_files) - set(verify_exclude)):
            temp_filename = os.path.join(temp_dir, filename)
            out_filename = os.path.join(out_dir, filename)
            if filename not in repo_files:
                print('ERROR: Missing repo file', filename)
                return 2
            elif filename not in temp_files:
                print('ERROR: Missing generator for', filename)
                return 3
            elif not filecmp.cmp(temp_filename, out_filename, shallow=False):
                print('ERROR: Repo files do not match generator output for', filename)
                # print line diff on file mismatch
                with open(temp_filename) as temp_file, open(out_filename) as repo_file:
                    print(''.join(difflib.unified_diff(temp_file.readlines(),
                                                       repo_file.readlines(),
                                                       fromfile='temp/' + filename,
                                                       tofile=  'repo/' + filename)))
                return 4

        # Return code for test scripts
        print('SUCCESS: Repo files match generator output')

    elif args.incremental:
        # copy missing or differing files from temp directory to repo
        for filename in os.listdir(temp_dir):
            temp_filename = os.path.join(temp_dir, filename)
            out_filename = os.path.join(out_dir, filename)
            if not os.path.exists(out_filename) or \
               not filecmp.cmp(temp_filename, out_filename, shallow=False):
                print('update', out_filename)
                shutil.copyfile(temp_filename, out_filename)

    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))

