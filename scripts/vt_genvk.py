#!/usr/bin/python3
#
# Copyright (c) 2013-2018 The Khronos Group Inc.
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

import argparse, cProfile, pdb, string, sys, time, os

# Simple timer functions
startTime = None

def startTimer(timeit):
    global startTime
    if timeit:
        startTime = time.process_time()

def endTimer(timeit, msg):
    global startTime
    if timeit:
        endTime = time.process_time()
        write(msg, endTime - startTime, file=sys.stderr)
        startTime = None

# Turn a list of strings into a regexp string matching exactly those strings
def makeREstring(list, default = None):
    if len(list) > 0 or default is None:
        return '^(' + '|'.join(list) + ')$'
    else:
        return default

# Returns a directory of [ generator function, generator options ] indexed
# by specified short names. The generator options incorporate the following
# parameters:
#
# args is an parsed argument object; see below for the fields that are used.
def makeGenOpts(args):
    global genOpts
    genOpts = {}

    # Default class of extensions to include, or None
    defaultExtensions = args.defaultExtensions

    # Additional extensions to include (list of extensions)
    extensions = args.extension

    # Extensions to remove (list of extensions)
    removeExtensions = args.removeExtensions

    # Extensions to emit (list of extensions)
    emitExtensions = args.emitExtensions

    # Features to include (list of features)
    features = args.feature

    # Whether to disable inclusion protect in headers
    protect = args.protect

    # Output target directory
    directory = args.directory

    # Path to generated files, particularly api.py
    genpath = args.genpath

    # Descriptive names for various regexp patterns used to select
    # versions and extensions
    allFeatures     = allExtensions = '.*'
    noFeatures      = noExtensions = None

    # Turn lists of names/patterns into matching regular expressions
    addExtensionsPat     = makeREstring(extensions, None)
    removeExtensionsPat  = makeREstring(removeExtensions, None)
    emitExtensionsPat    = makeREstring(emitExtensions, allExtensions)
    featuresPat          = makeREstring(features, allFeatures)

    if len(features) > 0:
        features = makeREstring(features)
    else:
        features = allFeatures

    # write('* Selecting features: ', features, file=sys.stderr)

    # Copyright text prefixing all headers (list of strings).
    prefixStrings = [
        '/*',
        '** Copyright (c) 2015-2020 The Khronos Group Inc.',
        '**',
        '** Licensed under the Apache License, Version 2.0 (the "License");',
        '** you may not use this file except in compliance with the License.',
        '** You may obtain a copy of the License at',
        '**',
        '**     http://www.apache.org/licenses/LICENSE-2.0',
        '**',
        '** Unless required by applicable law or agreed to in writing, software',
        '** distributed under the License is distributed on an "AS IS" BASIS,',
        '** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.',
        '** See the License for the specific language governing permissions and',
        '** limitations under the License.',
        '*/',
        ''
    ]

    # Text specific to Vulkan headers
    vkPrefixStrings = [
        '/*',
        '** This header is generated from the Khronos Vulkan XML API Registry.',
        '**',
        '*/',
        ''
    ]

    # Defaults for generating re-inclusion protection wrappers (or not)
    protectFeature = protect

    # An API style convention object
    conventions = VulkanConventions()

    # Options for Vulkan Layer Factory header
    genOpts['layer_factory.h'] = [
          LayerFactoryOutputGenerator,
          LayerFactoryGeneratorOptions(
            conventions       = conventions,
            filename          = 'layer_factory.h',
            directory         = directory,
            genpath           = None,
            apiname           = 'vulkan',
            profile           = None,
            versions          = featuresPat,
            emitversions      = featuresPat,
            defaultExtensions = 'vulkan',
            addExtensions     = addExtensionsPat,
            removeExtensions  = removeExtensionsPat,
            emitExtensions    = emitExtensionsPat,
            prefixText        = prefixStrings + vkPrefixStrings,
            apicall           = 'VKAPI_ATTR ',
            apientry          = 'VKAPI_CALL ',
            apientryp         = 'VKAPI_PTR *',
            alignFuncParam    = 48,
            helper_file_type  = 'layer_factory_header',
            expandEnumerants = False)
        ]

    # Options for Vulkan Layer Factory source file
    genOpts['layer_factory.cpp'] = [
          LayerFactoryOutputGenerator,
          LayerFactoryGeneratorOptions(
            conventions       = conventions,
            filename          = 'layer_factory.cpp',
            directory         = directory,
            genpath           = None,
            apiname           = 'vulkan',
            profile           = None,
            versions          = featuresPat,
            emitversions      = featuresPat,
            defaultExtensions = 'vulkan',
            addExtensions     = addExtensionsPat,
            removeExtensions  = removeExtensionsPat,
            emitExtensions    = emitExtensionsPat,
            prefixText        = prefixStrings + vkPrefixStrings,
            apicall           = 'VKAPI_ATTR ',
            apientry          = 'VKAPI_CALL ',
            apientryp         = 'VKAPI_PTR *',
            alignFuncParam    = 48,
            helper_file_type  = 'layer_factory_source',
            expandEnumerants = False)
        ]

    # API dump generator options for api_dump.cpp
    genOpts['api_dump.cpp'] = [
        ApiDumpOutputGenerator,
        ApiDumpGeneratorOptions(
            conventions       = conventions,
            input             = COMMON_CODEGEN,
            filename          = 'api_dump.cpp',
            apiname           = 'vulkan',
            genpath           = None,
            profile           = None,
            versions          = featuresPat,
            emitversions      = featuresPat,
            defaultExtensions = 'vulkan',
            addExtensions     = addExtensionsPat,
            removeExtensions  = removeExtensionsPat,
            emitExtensions    = emitExtensionsPat,
            prefixText        = prefixStrings + vkPrefixStrings,
            genFuncPointers   = True,
            protectFile       = protect,
            protectFeature    = False,
            protectProto      = None,
            protectProtoStr   = 'VK_NO_PROTOTYPES',
            apicall           = 'VKAPI_ATTR ',
            apientry          = 'VKAPI_CALL ',
            apientryp         = 'VKAPI_PTR *',
            alignFuncParam    = 48,
            expandEnumerants = False)
        ]

    # API dump generator options for api_dump_text.h
    genOpts['api_dump_text.h'] = [
        ApiDumpOutputGenerator,
        ApiDumpGeneratorOptions(
            conventions       = conventions,
            input             = TEXT_CODEGEN,
            filename          = 'api_dump_text.h',
            apiname           = 'vulkan',
            genpath           = None,
            profile           = None,
            versions          = featuresPat,
            emitversions      = featuresPat,
            defaultExtensions = 'vulkan',
            addExtensions     = addExtensionsPat,
            removeExtensions  = removeExtensionsPat,
            emitExtensions    = emitExtensionsPat,
            prefixText        = prefixStrings + vkPrefixStrings,
            genFuncPointers   = True,
            protectFile       = protect,
            protectFeature    = False,
            protectProto      = None,
            protectProtoStr   = 'VK_NO_PROTOTYPES',
            apicall           = 'VKAPI_ATTR ',
            apientry          = 'VKAPI_CALL ',
            apientryp         = 'VKAPI_PTR *',
            alignFuncParam    = 48,
            expandEnumerants  = False)
    ]

    # API dump generator options for api_dump_html.h
    genOpts['api_dump_html.h'] = [
        ApiDumpOutputGenerator,
        ApiDumpGeneratorOptions(
            conventions       = conventions,
            input             = HTML_CODEGEN,
            filename          = 'api_dump_html.h',
            apiname           = 'vulkan',
            genpath           = None,
            profile           = None,
            versions          = featuresPat,
            emitversions      = featuresPat,
            defaultExtensions = 'vulkan',
            addExtensions     = addExtensionsPat,
            removeExtensions  = removeExtensionsPat,
            emitExtensions    = emitExtensionsPat,
            prefixText        = prefixStrings + vkPrefixStrings,
            genFuncPointers   = True,
            protectFile       = protect,
            protectFeature    = False,
            protectProto      = None,
            protectProtoStr   = 'VK_NO_PROTOTYPES',
            apicall           = 'VKAPI_ATTR ',
            apientry          = 'VKAPI_CALL ',
            apientryp         = 'VKAPI_PTR *',
            alignFuncParam    = 48,
            expandEnumerants  = False)
    ]

    # API dump generator options for api_dump_json.h
    genOpts['api_dump_json.h'] = [
        ApiDumpOutputGenerator,
        ApiDumpGeneratorOptions(
            conventions       = conventions,
            input             = JSON_CODEGEN,
            filename          = 'api_dump_json.h',
            apiname           = 'vulkan',
            genpath           = None,
            profile           = None,
            versions          = featuresPat,
            emitversions      = featuresPat,
            defaultExtensions = 'vulkan',
            addExtensions     = addExtensionsPat,
            removeExtensions  = removeExtensionsPat,
            emitExtensions    = emitExtensionsPat,
            prefixText        = prefixStrings + vkPrefixStrings,
            genFuncPointers   = True,
            protectFile       = protect,
            protectFeature    = False,
            protectProto      = None,
            protectProtoStr   = 'VK_NO_PROTOTYPES',
            apicall           = 'VKAPI_ATTR ',
            apientry          = 'VKAPI_CALL ',
            apientryp         = 'VKAPI_PTR *',
            alignFuncParam    = 48,
            expandEnumerants  = False)
    ]

    # Helper file generator options for vk_struct_size_helper.h
    genOpts['vk_struct_size_helper.h'] = [
          ToolHelperFileOutputGenerator,
          ToolHelperFileOutputGeneratorOptions(
            conventions       = conventions,
            filename          = 'vk_struct_size_helper.h',
            directory         = directory,
            apiname           = 'vulkan',
            genpath           = None,
            profile           = None,
            versions          = featuresPat,
            emitversions      = featuresPat,
            defaultExtensions = 'vulkan',
            addExtensions     = addExtensionsPat,
            removeExtensions  = removeExtensionsPat,
            emitExtensions    = emitExtensionsPat,
            prefixText        = prefixStrings + vkPrefixStrings,
            protectFeature    = False,
            apicall           = 'VKAPI_ATTR ',
            apientry          = 'VKAPI_CALL ',
            apientryp         = 'VKAPI_PTR *',
            alignFuncParam    = 48,
            helper_file_type  = 'struct_size_header')
        ]

    # Helper file generator options for vk_struct_size_helper.c
    genOpts['vk_struct_size_helper.c'] = [
          ToolHelperFileOutputGenerator,
          ToolHelperFileOutputGeneratorOptions(
            conventions       = conventions,
            filename          = 'vk_struct_size_helper.c',
            directory         = directory,
            apiname           = 'vulkan',
            genpath           = None,
            profile           = None,
            versions          = featuresPat,
            emitversions      = featuresPat,
            defaultExtensions = 'vulkan',
            addExtensions     = addExtensionsPat,
            removeExtensions  = removeExtensionsPat,
            emitExtensions    = emitExtensionsPat,
            prefixText        = prefixStrings + vkPrefixStrings,
            protectFeature    = False,
            apicall           = 'VKAPI_ATTR ',
            apientry          = 'VKAPI_CALL ',
            apientryp         = 'VKAPI_PTR *',
            alignFuncParam    = 48,
            helper_file_type  = 'struct_size_source')
        ]

# Create an API generator and corresponding generator options based on
# the requested target and command line options.
# This is encapsulated in a function so it can be profiled and/or timed.
# The args parameter is an parsed argument object containing the following
# fields that are used:
#   target - target to generate
#   directory - directory to generate it in
#   protect - True if re-inclusion wrappers should be created
#   extensions - list of additional extensions to include in generated
#   interfaces
def genTarget(args):
    global genOpts

    # Create generator options with parameters specified on command line
    makeGenOpts(args)

    # Select a generator matching the requested target
    if (args.target in genOpts.keys()):
        createGenerator = genOpts[args.target][0]
        options = genOpts[args.target][1]

        #if not args.quiet:
           # write('* Building', options.filename, file=sys.stderr)
           # write('* options.versions          =', options.versions, file=sys.stderr)
           # write('* options.emitversions      =', options.emitversions, file=sys.stderr)
           # write('* options.defaultExtensions =', options.defaultExtensions, file=sys.stderr)
           # write('* options.addExtensions     =', options.addExtensions, file=sys.stderr)
           # write('* options.removeExtensions  =', options.removeExtensions, file=sys.stderr)
           # write('* options.emitExtensions    =', options.emitExtensions, file=sys.stderr)

        gen = createGenerator(errFile=errWarn,
                              warnFile=errWarn,
                              diagFile=diag)
        if not args.quiet:
            write('* Generated', options.filename, file=sys.stderr)
        return (gen, options)
    else:
        write('No generator options for unknown target:', args.target, file=sys.stderr)
        return None

# -feature name
# -extension name
# For both, "name" may be a single name, or a space-separated list
# of names, or a regular expression.
if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    parser.add_argument('-defaultExtensions', action='store',
                        default='vulkan',
                        help='Specify a single class of extensions to add to targets')
    parser.add_argument('-extension', action='append',
                        default=[],
                        help='Specify an extension or extensions to add to targets')
    parser.add_argument('-removeExtensions', action='append',
                        default=[],
                        help='Specify an extension or extensions to remove from targets')
    parser.add_argument('-emitExtensions', action='append',
                        default=[],
                        help='Specify an extension or extensions to emit in targets')
    parser.add_argument('-feature', action='append',
                        default=[],
                        help='Specify a core API feature name or names to add to targets')
    parser.add_argument('-debug', action='store_true',
                        help='Enable debugging')
    parser.add_argument('-dump', action='store_true',
                        help='Enable dump to stderr')
    parser.add_argument('-diagfile', action='store',
                        default=None,
                        help='Write diagnostics to specified file')
    parser.add_argument('-errfile', action='store',
                        default=None,
                        help='Write errors and warnings to specified file instead of stderr')
    parser.add_argument('-noprotect', dest='protect', action='store_false',
                        help='Disable inclusion protection in output headers')
    parser.add_argument('-profile', action='store_true',
                        help='Enable profiling')
    parser.add_argument('-registry', action='store',
                        default='vk.xml',
                        help='Use specified registry file instead of vk.xml')
    parser.add_argument('-time', action='store_true',
                        help='Enable timing')
    parser.add_argument('-validate', action='store_true',
                        help='Enable group validation')
    parser.add_argument('-genpath', action='store', default='gen',
                        help='Path to generated files')
    parser.add_argument('-o', action='store', dest='directory',
                        default='.',
                        help='Create target and related files in specified directory')
    parser.add_argument('target', metavar='target', nargs='?',
                        help='Specify target')
    parser.add_argument('-quiet', action='store_true', default=False,
                        help='Suppress script output during normal execution.')

    # This argument tells us where to load the script from the Vulkan-Headers registry
    parser.add_argument('-scripts', action='store',
                        help='Find additional scripts in this directory')

    args = parser.parse_args()

    scripts_directory_path = os.path.dirname(os.path.abspath(__file__))
    registry_headers_path = os.path.join(scripts_directory_path, args.scripts)
    sys.path.insert(0, registry_headers_path)

    from reg import *
    from generator import *
    from cgenerator import CGeneratorOptions, COutputGenerator

    # VulkanTools generator additions
    from tool_helper_file_generator import ToolHelperFileOutputGenerator, ToolHelperFileOutputGeneratorOptions
    from api_dump_generator import ApiDumpGeneratorOptions, ApiDumpOutputGenerator, COMMON_CODEGEN, TEXT_CODEGEN, HTML_CODEGEN, JSON_CODEGEN
    from layer_factory_generator import LayerFactoryGeneratorOptions, LayerFactoryOutputGenerator
    from vkconventions import VulkanConventions

    # This splits arguments which are space-separated lists
    args.feature = [name for arg in args.feature for name in arg.split()]
    args.extension = [name for arg in args.extension for name in arg.split()]

    # create error/warning & diagnostic files
    if (args.errfile):
        errWarn = open(args.errfile, 'w', encoding='utf-8')
    else:
        errWarn = sys.stderr

    if (args.diagfile):
        diag = open(args.diagfile, 'w', encoding='utf-8')
    else:
        diag = None

    # Create the API generator & generator options
    (gen, options) = genTarget(args)

    # Create the registry object with the specified generator and generator
    # options. The options are set before XML loading as they may affect it.
    reg = Registry(gen, options)

    # Parse the specified registry XML into an ElementTree object
    startTimer(args.time)
    tree = etree.parse(args.registry)
    endTimer(args.time, '* Time to make ElementTree =')

    # Load the XML tree into the registry object
    startTimer(args.time)
    reg.loadElementTree(tree)
    endTimer(args.time, '* Time to parse ElementTree =')

    if (args.validate):
        reg.validateGroups()

    if (args.dump):
        write('* Dumping registry to regdump.txt', file=sys.stderr)
        reg.dumpReg(filehandle = open('regdump.txt', 'w', encoding='utf-8'))

    # Finally, use the output generator to create the requested target
    if (args.debug):
        pdb.run('reg.apiGen()')
    else:
        startTimer(args.time)
        reg.apiGen()
        endTimer(args.time, '* Time to generate ' + options.filename + ' =')

    if not args.quiet:
        write('* Generated ', options.filename, file=sys.stderr)
