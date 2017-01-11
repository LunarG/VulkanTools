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

python3 ../scripts/vk_helper.py --gen_struct_wrappers ../include/vulkan/vulkan.h --abs_out_dir generated/include

( cd generated/include; python3 ../../../scripts/lvl_genvk.py -registry ../../../scripts/vk.xml vk_struct_size_helper.h )
( cd generated/include; python3 ../../../scripts/lvl_genvk.py -registry ../../../scripts/vk.xml vk_struct_size_helper.c )
( cd generated/include; python3 ../../../scripts/lvl_genvk.py -registry ../../../scripts/vk.xml vk_enum_string_helper.h )
( cd generated/include; python3 ../../../scripts/lvl_genvk.py -registry ../../../scripts/vk.xml vk_dispatch_table_helper.h )
( cd generated/include; python3 ../../../scripts/lvl_genvk.py -registry ../../../scripts/vk.xml thread_check.h )
( cd generated/include; python3 ../../../scripts/lvl_genvk.py -registry ../../../scripts/vk.xml parameter_validation.h )
( cd generated/include; python3 ../../../scripts/lvl_genvk.py -registry ../../../scripts/vk.xml unique_objects_wrappers.h )

( cd generated/include; python3 ../../../scripts/vt_genvk.py -registry ../../../scripts/vk.xml api_dump.cpp )
( cd generated/include; python3 ../../../scripts/vt_genvk.py -registry ../../../scripts/vk.xml api_dump_text.h )

# vktrace
python3 ../vktrace/vktrace_generate.py AllPlatforms vktrace-trace-h vk_version_1_0 > generated/include/vktrace_vk_vk.h
python3 ../vktrace/vktrace_generate.py AllPlatforms vktrace-trace-c vk_version_1_0 > generated/include/vktrace_vk_vk.cpp
python3 ../vktrace/vktrace_generate.py AllPlatforms vktrace-core-trace-packets vk_version_1_0 > generated/include/vktrace_vk_vk_packets.h
python3 ../vktrace/vktrace_generate.py AllPlatforms vktrace-packet-id vk_version_1_0 > generated/include/vktrace_vk_packet_id.h

# vkreplay
python3 ../vktrace/vktrace_generate.py AllPlatforms vktrace-replay-vk-funcs vk_version_1_0 > generated/include/vkreplay_vk_func_ptrs.h
python3 ../vktrace/vktrace_generate.py AllPlatforms vktrace-replay-c vk_version_1_0 > generated/include/vkreplay_vk_replay_gen.cpp
python3 ../vktrace/vktrace_generate.py AllPlatforms vktrace-replay-obj-mapper-h vk_version_1_0 > generated/include/vkreplay_vk_objmapper.h

cp -f ../layers/vk_layer_config.cpp   generated/common/
cp -f ../layers/vk_layer_extension_utils.cpp  generated/common/
cp -f ../layers/vk_layer_utils.cpp    generated/common/
cp -f ../layers/vk_layer_table.cpp    generated/common/
cp -f ../layers/descriptor_sets.cpp   generated/common/

# layer names and their original source files directory
# 1 to 1 correspondence -- one layer one source file; additional files are copied
# at fixup step
declare layers=(core_validation image object_tracker parameter_validation swapchain threading unique_objects api_dump screenshot)
declare src_dirs=(../layers ../layers ../layers ../layers ../layers ../layers ../layers generated/include ../layersvt)

SRC_ROOT=generated/layer-src
BUILD_ROOT=generated/gradle-build

# create build-script root directory
cp -fr gradle-templates   generated/gradle-build
for ((i = 0; i < ${#layers[@]}; i++))
do
#   copy the sources
    mkdir  -p ${SRC_ROOT}/${layers[i]}
    cp -f ${src_dirs[i]}/${layers[i]}.cpp  ${SRC_ROOT}/${layers[i]}/

#   copy build scripts
    mkdir -p ${BUILD_ROOT}/${layers[i]}
    echo "apply from: \"../common.gradle\"" > ${BUILD_ROOT}/${layers[i]}/build.gradle
done

# fixup - unique_objects need one more file
cp  generated/common/descriptor_sets.cpp ${SRC_ROOT}/core_validation/descriptor_sets.cpp
cp  generated/include/vk_safe_struct.cpp ${SRC_ROOT}/core_validation/vk_safe_struct.cpp
mv  generated/include/vk_safe_struct.cpp ${SRC_ROOT}/unique_objects/vk_safe_struct.cpp

# fixup - remove copied files from generated/include
rm  generated/include/api_dump.cpp

exit 0
