@echo off
REM # Copyright 2015-2018 The Android Open Source Project
REM # Copyright (C) 2015-2018 Valve Corporation
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

echo Removing old generated directory in VT\build-android
echo ********
if exist generated (
    RMDIR /S /Q generated
)

echo Creating new empty generated directories in VT\build-android
echo ********
MKDIR generated
MKDIR generated\include
MKDIR generated\common

echo Setting some environment variables
echo ********
set LVL_BASE=..\submodules\Vulkan-ValidationLayers
set LVL_SCRIPTS=..\..\%LVL_BASE%\scripts
set VT_SCRIPTS=..\..\..\scripts
set REGISTRY=..\..\%LVL_BASE%\scripts\vk.xml

echo Entering Generated/Include Folder
echo ********
pushd generated\include

echo Generating LVL header/source files
echo ********
py -3 %LVL_SCRIPTS%\lvl_genvk.py -registry %REGISTRY% vk_safe_struct.h
py -3 %LVL_SCRIPTS%\lvl_genvk.py -registry %REGISTRY% vk_safe_struct.cpp
py -3 %LVL_SCRIPTS%\lvl_genvk.py -registry %REGISTRY% vk_enum_string_helper.h
py -3 %LVL_SCRIPTS%\lvl_genvk.py -registry %REGISTRY% vk_object_types.h
py -3 %LVL_SCRIPTS%\lvl_genvk.py -registry %REGISTRY% vk_dispatch_table_helper.h
py -3 %LVL_SCRIPTS%\lvl_genvk.py -registry %REGISTRY% thread_check.h
py -3 %LVL_SCRIPTS%\lvl_genvk.py -registry %REGISTRY% parameter_validation.cpp
py -3 %LVL_SCRIPTS%\lvl_genvk.py -registry %REGISTRY% unique_objects_wrappers.h
py -3 %LVL_SCRIPTS%\lvl_genvk.py -registry %REGISTRY% vk_layer_dispatch_table.h
py -3 %LVL_SCRIPTS%\lvl_genvk.py -registry %REGISTRY% vk_extension_helper.h
py -3 %LVL_SCRIPTS%\lvl_genvk.py -registry %REGISTRY% object_tracker.cpp
py -3 %LVL_SCRIPTS%\lvl_genvk.py -registry %REGISTRY% vk_typemap_helper.h
py -3 %LVL_SCRIPTS%\external_revision_generator.py --git_dir ..\..\third_party\shaderc\third_party\spirv-tools -s SPIRV_TOOLS_COMMIT_ID -o spirv_tools_commit_id.h

REM layer factory
echo Generating VT layer factory header/source files
echo ********
py -3 %VT_SCRIPTS%\vt_genvk.py -registry %REGISTRY% layer_factory.h
py -3 %VT_SCRIPTS%\vt_genvk.py -registry %REGISTRY% layer_factory.cpp
py -3 %VT_SCRIPTS%\vlf_makefile_generator.py ..\..\..\layer_factory

REM apidump
echo Generating VT apidump header/source files
echo ********
py -3 %VT_SCRIPTS%\vt_genvk.py -registry %REGISTRY% api_dump.cpp
py -3 %VT_SCRIPTS%\vt_genvk.py -registry %REGISTRY% api_dump_text.h
py -3 %VT_SCRIPTS%\vt_genvk.py -registry %REGISTRY% api_dump_html.h

REM vktrace
echo Generating VT vktrace header/source files
echo ********
py -3 %VT_SCRIPTS%\vt_genvk.py -registry %REGISTRY% vktrace_vk_vk.h
py -3 %VT_SCRIPTS%\vt_genvk.py -registry %REGISTRY% vktrace_vk_vk.cpp
py -3 %VT_SCRIPTS%\vt_genvk.py -registry %REGISTRY% vktrace_vk_vk_packets.h
py -3 %VT_SCRIPTS%\vt_genvk.py -registry %REGISTRY% vktrace_vk_packet_id.h
py -3 %VT_SCRIPTS%\vt_genvk.py -registry %REGISTRY% vk_struct_size_helper.h
py -3 %VT_SCRIPTS%\vt_genvk.py -registry %REGISTRY% vk_struct_size_helper.c

REM vkreplay
echo Generating VT vkreplay header/source files
echo ********
py -3 %VT_SCRIPTS%\vt_genvk.py -registry %REGISTRY% vkreplay_vk_func_ptrs.h
py -3 %VT_SCRIPTS%\vt_genvk.py -registry %REGISTRY% vkreplay_vk_replay_gen.cpp
py -3 %VT_SCRIPTS%\vt_genvk.py -registry %REGISTRY% vkreplay_vk_objmapper.h

REM Copy over the built source files to the LVL submodule.  Otherwise,
REM cube won't build.
echo Entering submodules\Vulkan-ValidationLayers\build-android
echo ********
pushd ..\..\%LVL_BASE%\build-android
REM Removing old generated directory in submodules\Vulkan-ValidationLayers\build-android
if exist generated (
    RMDIR /S /Q generated
)
REM Creating new empty generated directories in submodules\Vulkan-ValidationLayers\build-android
MKDIR generated
MKDIR generated\include
MKDIR generated\common

echo Leaving submodules\Vulkan-ValidationLayers\build-android
echo ********
popd

echo Copying generated headers/source into submodules\Vulkan-ValidationLayers\build-android
echo ********
COPY /Y * ..\..\%LVL_BASE%\build-android\generated\include

echo leaving Generated/Include Folder
echo ********
popd
