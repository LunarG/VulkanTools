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

# layer factory
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} layer_factory.h )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} layer_factory.cpp )
( cd generated/include; python3 ${VT_SCRIPTS}/vlf_makefile_generator.py ../../../layer_factory ${REGISTRY_PATH}/../include)

# apidump
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} api_dump.cpp )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} api_dump_text.h )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} api_dump_html.h )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} api_dump_json.h )
 
# vktrace
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} vktrace_vk_vk.h )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} vktrace_vk_vk.cpp )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} vktrace_vk_vk_packets.h )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} vktrace_vk_packet_id.h )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} vk_struct_size_helper.h )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} vk_struct_size_helper.c )

# vkreplay
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} vkreplay_vk_func_ptrs.h )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} vkreplay_vk_replay_gen.cpp )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} -scripts ${REGISTRY_PATH} vkreplay_vk_objmapper.h )

( pushd ${LVL_BASE}/build-android; rm -rf generated; mkdir -p generated/include generated/common; popd )
( cd generated/include; cp -rf * ${LVL_BASE}/build-android/generated/include )

exit 0
