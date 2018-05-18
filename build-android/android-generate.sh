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

LVL_BASE=../submodules/Vulkan-ValidationLayers
LVL_SCRIPTS=../../${LVL_BASE}/scripts
VT_SCRIPTS=../../../scripts
REGISTRY=../../${LVL_BASE}/scripts/vk.xml

( cd generated/include; python3 ${LVL_SCRIPTS}/lvl_genvk.py -registry ${REGISTRY} vk_safe_struct.h )
( cd generated/include; python3 ${LVL_SCRIPTS}/lvl_genvk.py -registry ${REGISTRY} vk_safe_struct.cpp )
( cd generated/include; python3 ${LVL_SCRIPTS}/lvl_genvk.py -registry ${REGISTRY} vk_enum_string_helper.h )
( cd generated/include; python3 ${LVL_SCRIPTS}/lvl_genvk.py -registry ${REGISTRY} vk_object_types.h )
( cd generated/include; python3 ${LVL_SCRIPTS}/lvl_genvk.py -registry ${REGISTRY} vk_dispatch_table_helper.h )
( cd generated/include; python3 ${LVL_SCRIPTS}/lvl_genvk.py -registry ${REGISTRY} thread_check.h )
( cd generated/include; python3 ${LVL_SCRIPTS}/lvl_genvk.py -registry ${REGISTRY} parameter_validation.cpp )
( cd generated/include; python3 ${LVL_SCRIPTS}/lvl_genvk.py -registry ${REGISTRY} unique_objects_wrappers.h )
( cd generated/include; python3 ${LVL_SCRIPTS}/lvl_genvk.py -registry ${REGISTRY} vk_loader_extensions.h )
( cd generated/include; python3 ${LVL_SCRIPTS}/lvl_genvk.py -registry ${REGISTRY} vk_loader_extensions.c )
( cd generated/include; python3 ${LVL_SCRIPTS}/lvl_genvk.py -registry ${REGISTRY} vk_layer_dispatch_table.h )
( cd generated/include; python3 ${LVL_SCRIPTS}/lvl_genvk.py -registry ${REGISTRY} vk_extension_helper.h )
( cd generated/include; python3 ${LVL_SCRIPTS}/lvl_genvk.py -registry ${REGISTRY} object_tracker.cpp )
( cd generated/include; python3 ${LVL_SCRIPTS}/lvl_genvk.py -registry ${REGISTRY} vk_typemap_helper.h )
( cd generated/include; python3 ${LVL_SCRIPTS}/external_revision_generator.py --git_dir ../../third_party/shaderc/third_party/spirv-tools -s SPIRV_TOOLS_COMMIT_ID -o spirv_tools_commit_id.h )

# layer factory
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} layer_factory.h )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} layer_factory.cpp )
( cd generated/include; python3 ${VT_SCRIPTS}/vlf_makefile_generator.py ../../../layer_factory )

# apidump
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} api_dump.cpp )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} api_dump_text.h )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} api_dump_html.h )

# vktrace
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} vktrace_vk_vk.h )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} vktrace_vk_vk.cpp )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} vktrace_vk_vk_packets.h )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} vktrace_vk_packet_id.h )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} vk_struct_size_helper.h )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} vk_struct_size_helper.c )

# vkreplay
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} vkreplay_vk_func_ptrs.h )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} vkreplay_vk_replay_gen.cpp )
( cd generated/include; python3 ${VT_SCRIPTS}/vt_genvk.py -registry ${REGISTRY} vkreplay_vk_objmapper.h )

( pushd ${LVL_BASE}/build-android; rm -rf generated; mkdir -p generated/include generated/common; popd )
( cd generated/include; cp -rf * ../../${LVL_BASE}/build-android/generated/include )

exit 0
