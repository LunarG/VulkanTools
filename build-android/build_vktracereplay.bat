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
call update_external_sources_android.bat
call android-generate.bat
call ndk-build -j %num_cpus%

if %errorlevel% neq 0 exit /b %errorlevel%

REM
REM create vkreplay APK
REM
call android update project -s -p . -t "android-23"
call ant -buildfile vkreplay debug

if %errorlevel% neq 0 exit /b %errorlevel%

REM
REM build cube-with-layers
REM
pushd ..\demos\android
call android update project -s -p . -t "android-23"
call ndk-build -j  %num_cpus%
call ant -buildfile cube-with-layers debug
popd

if %errorlevel% neq 0 exit /b %errorlevel%

REM
REM build vktrace
REM
pushd ..
call update_external_sources.bat --all
call build_windows_targets.bat
popd

if %errorlevel% neq 0 exit /b %errorlevel%

exit /b 0