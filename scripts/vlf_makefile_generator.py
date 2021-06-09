#!/usr/bin/env python3
#
# Copyright (c) 2015-2017 Valve Corporation
# Copyright (c) 2015-2017 LunarG, Inc.
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
# Author: Mark Lobodzinski <mark@lunarg.com>

import os
import subprocess
import sys
from glob import glob

if __name__ == '__main__':
    if (len(sys.argv) != 3):
        print("Usage: %s <VLLF_DIR> <VULKAN_HEADERS_INCLUDE_DIR" % sys.argv[0])
        sys.exit(os.EX_USAGE)

    # Write commit ID to output header file
    with open("Android.mk", "w") as header_file:

        # File Comment
        file_comment = '# *** THIS FILE IS GENERATED - DO NOT EDIT ***\n'
        file_comment += '# See vlf_makefile_generator.py for modifications\n'

        header_file.write(file_comment)
        # Copyright Notice
        copyright = '\n'
        copyright += '#  Copyright (c) 2015-2017 The Khronos Group Inc.\n'
        copyright += '#  Copyright (c) 2015-2017 Valve Corporation\n'
        copyright += '#  Copyright (c) 2015-2017 LunarG, Inc.\n'
        copyright += '#  Copyright (c) 2015-2017 Google Inc.\n'
        copyright += '#\n'
        copyright += '#  Licensed under the Apache License, Version 2.0 (the "License");\n'
        copyright += '#  you may not use this file except in compliance with the License.\n'
        copyright += '#  You may obtain a copy of the License at\n'
        copyright += '#\n'
        copyright += '#      http://www.apache.org/licenses/LICENSE-2.0\n'
        copyright += '#\n'
        copyright += '#  Unless required by applicable law or agreed to in writing, software\n'
        copyright += '#  distributed under the License is distributed on an "AS IS" BASIS,\n'
        copyright += '#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n'
        copyright += '#  See the License for the specific language governing permissions and\n'
        copyright += '#  limitations under the License.\n'
        copyright += '#\n'
        copyright += '#  Author: Mark Lobodzinski <mark@lunarg.com>\n'
        copyright += '#\n\n'
        header_file.write(copyright)

        # Destination directory is first (and only) arg
        vlf_dir = sys.argv[1]
        include_dir = sys.argv[2]
        include_dir = os.path.normpath(include_dir)

        # Get list of subdirectories in layer_factory (dest) dir
        layer_factory_path = "%s/*/" % vlf_dir
        layer_factory_dirs = glob(layer_factory_path)

        contents = ''

        # Output makefile target section for each factory layer in dest sub-directory
        for layer_factory_path in layer_factory_dirs:
            factory_layer = os.path.basename(os.path.normpath(layer_factory_path))
            contents += '\n'
            contents += 'include $(CLEAR_VARS)\n'
            contents += 'LOCAL_MODULE := VkLayer_%s\n' % factory_layer
            contents += 'LOCAL_SRC_FILES += $(LAYER_DIR)/include/layer_factory.cpp\n'
            contents += 'LOCAL_SRC_FILES += $(LVL_DIR)/layers/xxhash.c\n'
            # Add *.cpp files (if any) to makefile dependencies
            for path, subdirs, files in os.walk(factory_layer):
                for file in files:
                    if '.cpp' in file:
                        contents += 'LOCAL_SRC_FILES += $(LOCAL_PATH)/$(SRC_DIR)/layer_factory/%s/%s\n' & (factory_layer, file)
            contents += 'LOCAL_C_INCLUDES += %s\n' % include_dir
            contents += 'LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(LAYER_DIR)/include\n'
            contents += 'LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(LVL_DIR)/layers\n'
            contents += 'LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(LVL_DIR)/layers/generated\n'
            contents += 'LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SRC_DIR)/layer_factory/%s\n' % factory_layer
            contents += 'LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(LVL_DIR)/loader\n'
            contents += 'LOCAL_STATIC_LIBRARIES += layer_utils\n'
            contents += 'LOCAL_CPPFLAGS += -std=c++11 -DVK_PROTOTYPES -Wall -Werror -Wno-unused-function -Wno-unused-const-variable\n'
            contents += 'LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR -DVK_ENABLE_BETA_EXTENSIONS -fvisibility=hidden\n'
            contents += 'LOCAL_LDLIBS    := -llog\n'
            contents += 'LOCAL_LDFLAGS   += -Wl,-Bsymbolic\n'
            contents += 'LOCAL_LDFLAGS   += -Wl,--exclude-libs,ALL\n'
            contents += 'include $(BUILD_SHARED_LIBRARY)\n'


        header_file.write(contents)

