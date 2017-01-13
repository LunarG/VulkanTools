@echo off
REM # Copyright 2016 The Android Open Source Project
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

set num_cpus=%NUMBER_OF_PROCESSORS%

REM
REM build layers
REM
call update_external_sources_android.bat  || exit /b 1
call android-generate.bat                 || exit /b 1
call ndk-build -j %num_cpus%              || exit /b 1

REM
REM create vkreplay APK
REM
call android update project -s -p . -t "android-23" || exit /b 1
call ant -buildfile vkreplay debug                  || exit /b 1

REM
REM build cube-with-layers
REM
pushd ..\demos\android                               || exit /b 1
call android update project -s -p . -t "android-23"  || exit /b 1
call ndk-build -j  %num_cpus%                        || exit /b 1
call ant -buildfile cube-with-layers debug           || exit /b 1
popd

REM
REM build vktrace
REM
pushd ..

REM Attempt to use the current Visual Studio version
python .\scripts\determine_vs_version.py > vsversion.tmp  || exit /b 1
set /p VS_VERSION=< vsversion.tmp                         || exit /b 1
del /Q /F vsversion.tmp                                   || exit /b 1
echo VS_VERSION = %VS_VERSION%

REM build the desktop shader toolchain
call update_external_sources.bat --build-glslang --build-spirv-tools || exit /b 1

REM check for 64-bit Qt install
set Qt5_Dir_64=C:\Qt\5.6\msvc2013_64\lib\cmake\Qt5
if not exist "%Qt5_Dir_64%" echo Please set Qt5_Dir_64 to 64-bit Qt install location. && exit /b 1

REM check for 32-bit Qt install
set Qt5_Dir_32=C:\Qt\5.6\msvc2013\lib\cmake\Qt5
if not exist "%Qt5_Dir_32%" echo Please set Qt5_Dir_32 to 32-bit Qt install location. && exit /b 1

REM build 64-bit vktrace and vktraceviewer
if not exist "build\" mkdir build || exit /b 1
pushd build
set Qt5_Dir=%Qt5_Dir_64%
cmake -G "Visual Studio %VS_VERSION% Win64" ^
 -DBUILD_TESTS=Off ^
 -DBUILD_LAYERS=Off ^
 -DBUILD_LAYERSVT=Off ^
 -DBUILD_DEMOS=Off ^
 -DBUILD_VKJSON=Off ^
 -DBUILD_VIA=Off ^
 -DBUILD_VKTRACE_LAYER=Off ^
 -DBUILD_VKTRACE_REPLAY=Off ^
 -DBUILD_VKTRACE=On ^
 -DBUILD_LOADER=On ^
 -DBUILD_VKTRACEVIEWER=On ^
 .. || exit /b 1
echo Building 64-bit Debug
msbuild ALL_BUILD.vcxproj /p:Platform=x64 /p:Configuration=Debug /maxcpucount /verbosity:quiet   || exit /b 1
echo Building 64-bit Release
msbuild ALL_BUILD.vcxproj /p:Platform=x64 /p:Configuration=Release /maxcpucount /verbosity:quiet || exit /b 1
popd

REM build 32-bit vktrace and vktraceviewer
if not exist "build32\" mkdir build32 || exit /b 1
pushd build32
set Qt5_Dir=%Qt5_Dir_32%
cmake -G "Visual Studio %VS_VERSION%" ^
 -DBUILD_TESTS=Off ^
 -DBUILD_LAYERS=Off ^
 -DBUILD_LAYERSVT=Off ^
 -DBUILD_DEMOS=Off ^
 -DBUILD_VKJSON=Off ^
 -DBUILD_VIA=Off ^
 -DBUILD_VKTRACE_LAYER=Off ^
 -DBUILD_VKTRACE_REPLAY=Off ^
 -DBUILD_VKTRACE=On ^
 -DBUILD_LOADER=On ^
 -DBUILD_VKTRACEVIEWER=On ^
 .. || exit /b 1
echo Building 32-bit Debug
msbuild ALL_BUILD.vcxproj /p:Platform=x86 /p:Configuration=Debug /maxcpucount /verbosity:quiet   || exit /b 1
echo Building 32-bit Release
msbuild ALL_BUILD.vcxproj /p:Platform=x86 /p:Configuration=Release /maxcpucount /verbosity:quiet || exit /b 1
popd

REM done building vktrace
popd

exit /b 0