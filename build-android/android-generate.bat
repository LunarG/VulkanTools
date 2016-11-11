@echo off
REM # Copyright 2015 The Android Open Source Project
REM # Copyright (C) 2015 Valve Corporation
REM
REM # Licensed under the Apache License, Version 2.0 (the "License");
REM # you may not use this file except in compliance with the License.
REM # You may obtain a copy of the License at
REM
REM #      http://www.apache.org/licenses/LICENSE-2.0
REM
REM # Unless required by applicable law or agreed to in writing, software
REM # distributed under the License is distributed on an "AS IS" BASIS,
REM # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
REM # See the License for the specific language governing permissions and
REM # limitations under the License.

if exist generated (
  rmdir /s /q generated
)
mkdir generated\include generated\common

python ../vk-generate.py Android dispatch-table-ops layer > generated/include/vk_dispatch_table_helper.h

python ../vk_helper.py --gen_enum_string_helper ../include/vulkan/vulkan.h --abs_out_dir generated/include
python ../vk_helper.py --gen_struct_wrappers ../include/vulkan/vulkan.h --abs_out_dir generated/include

cd generated/include
python ../../../lvl_genvk.py -registry ../../../vk.xml thread_check.h
python ../../../lvl_genvk.py -registry ../../../vk.xml parameter_validation.h
python ../../../lvl_genvk.py -registry ../../../vk.xml unique_objects_wrappers.h
python ../../../vt_genvk.py -registry ../../../vk.xml api_dump.cpp

REM vktrace
python ../../../vktrace/vktrace_generate.py AllPlatforms vktrace-trace-h vk_version_1_0 > vktrace_vk_vk.h
python ../../../vktrace/vktrace_generate.py AllPlatforms vktrace-trace-c vk_version_1_0 > vktrace_vk_vk.cpp
python ../../../vktrace/vktrace_generate.py AllPlatforms vktrace-core-trace-packets vk_version_1_0 > vktrace_vk_vk_packets.h
python ../../../vktrace/vktrace_generate.py AllPlatforms vktrace-packet-id vk_version_1_0 > vktrace_vk_packet_id.h

REM vkreplay
python ../../../vktrace/vktrace_generate.py AllPlatforms vktrace-replay-vk-funcs vk_version_1_0 > vkreplay_vk_func_ptrs.h
python ../../../vktrace/vktrace_generate.py AllPlatforms vktrace-replay-c vk_version_1_0 > vkreplay_vk_replay_gen.cpp
python ../../../vktrace/vktrace_generate.py AllPlatforms vktrace-replay-obj-mapper-h vk_version_1_0 > vkreplay_vk_objmapper.h

cd ../..

copy /Y ..\layers\vk_layer_config.cpp   generated\common\
copy /Y ..\layers\vk_layer_extension_utils.cpp  generated\common\
copy /Y ..\layers\vk_layer_utils.cpp    generated\common\
copy /Y ..\layers\vk_layer_table.cpp    generated\common\
copy /Y ..\layers\descriptor_sets.cpp   generated\common\

REM create build-script root directory
mkdir generated\gradle-build
cd generated\gradle-build
mkdir  core_validation image object_tracker parameter_validation swapchain threading unique_objects api_dump screenshot
cd ..\..
mkdir generated\layer-src
cd generated\layer-src
mkdir  core_validation image object_tracker parameter_validation swapchain threading unique_objects api_dump screenshot
cd ..\..
xcopy /s gradle-templates\*   generated\gradle-build\
for %%G in (core_validation image object_tracker parameter_validation swapchain threading unique_objects) Do (
    copy ..\layers\%%G.cpp   generated\layer-src\%%G
    echo apply from: "../common.gradle"  > generated\gradle-build\%%G\build.gradle
)
for %%G in (screenshot) Do (
    copy ..\layersvt\%%G.cpp   generated\layer-src\%%G
    echo apply from: "../common.gradle"  > generated\gradle-build\%%G\build.gradle
)
copy generated\include\api_dump.cpp   generated\layer-src\api_dump
copy generated\common\descriptor_sets.cpp generated\layer-src\core_validation\descriptor_sets.cpp
copy generated\include\vk_safe_struct.cpp generated\layer-src\core_validation\vk_safe_struct.cpp
move generated\include\vk_safe_struct.cpp generated\layer-src\unique_objects\vk_safe_struct.cpp
echo apply from: "../common.gradle"  > generated\gradle-build\unique_objects\build.gradle

del  /f /q generated\include\api_dump.cpp
