#!/bin/bash

# Copyright 2015 The Android Open Source Project
# Copyright (C) 2015 Valve Corporation

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#      http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

dir=$(cd -P -- "$(dirname -- "$0")" && pwd -P)
cd $dir

rm -rf generated
mkdir -p generated/include generated/common

LVL_BASE=$dir/third_party/Vulkan-ValidationLayers
LVL_SCRIPTS=${LVL_BASE}/scripts
LVL_GENERATED=${LVL_BASE}/layers/generated
VT_SCRIPTS=../../../scripts
REGISTRY_PATH=$dir/third_party/Vulkan-Headers/registry
REGISTRY=${REGISTRY_PATH}/vk.xml

# Check for python 3.6 or greater
PYTHON_EXECUTABLE=python3
PYTHON_MINOR_VERSION=$($PYTHON_EXECUTABLE --version | cut -d. -f2)
if [ $PYTHON_MINOR_VERSION -lt "6" ]; then
    for MINOR_TEST in {6..20}
    do
        TEST_EXECUTABLE=$PYTHON_EXECUTABLE.$MINOR_TEST
        if which $TEST_EXECUTABLE; then
            PYTHON_EXECUTABLE=$TEST_EXECUTABLE
            break
        fi
    done
fi
echo "Using python: $(which $PYTHON_EXECUTABLE)"

# layer factory
( cd generated/include; ${PYTHON_EXECUTABLE} ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} layer_factory.h)
( cd generated/include; ${PYTHON_EXECUTABLE} ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} layer_factory.cpp)
( cd generated/include; ${PYTHON_EXECUTABLE} ${VT_SCRIPTS}/vlf_makefile_generator.py ../../../layer_factory ${REGISTRY_PATH}/../include)

# apidump
( cd generated/include; ${PYTHON_EXECUTABLE} ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} api_dump.cpp -removeExtensions VK_KHR_video_queue -removeExtensions VK_KHR_video_decode_queue -removeExtensions VK_KHR_video_encode_queue -removeExtensions VK_EXT_video_decode_h264 -removeExtensions VK_EXT_video_decode_h265 -removeExtensions VK_EXT_video_encode_h264 -removeExtensions VK_EXT_video_encode_h265)
( cd generated/include; ${PYTHON_EXECUTABLE} ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} api_dump_text.h -removeExtensions VK_KHR_video_queue -removeExtensions VK_KHR_video_decode_queue -removeExtensions VK_KHR_video_encode_queue -removeExtensions VK_EXT_video_decode_h264 -removeExtensions VK_EXT_video_decode_h265 -removeExtensions VK_EXT_video_encode_h264 -removeExtensions VK_EXT_video_encode_h265)
( cd generated/include; ${PYTHON_EXECUTABLE} ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} api_dump_html.h -removeExtensions VK_KHR_video_queue -removeExtensions VK_KHR_video_decode_queue -removeExtensions VK_KHR_video_encode_queue -removeExtensions VK_EXT_video_decode_h264 -removeExtensions VK_EXT_video_decode_h265 -removeExtensions VK_EXT_video_encode_h264 -removeExtensions VK_EXT_video_encode_h265)
( cd generated/include; ${PYTHON_EXECUTABLE} ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} api_dump_json.h -removeExtensions VK_KHR_video_queue -removeExtensions VK_KHR_video_decode_queue -removeExtensions VK_KHR_video_encode_queue -removeExtensions VK_EXT_video_decode_h264 -removeExtensions VK_EXT_video_decode_h265 -removeExtensions VK_EXT_video_encode_h264 -removeExtensions VK_EXT_video_encode_h265)
 
( pushd ${LVL_BASE}/build-android; rm -rf generated; mkdir -p generated/include generated/common; popd )
( cd generated/include; cp -rf * ${LVL_BASE}/build-android/generated/include )

exit 0
