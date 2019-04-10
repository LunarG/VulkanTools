@echo off
REM Update source for jsoncpp, Vulkan-Headers, Vulkan-Tools, and Vulkan-ValidationLayers

REM
REM Copyright 2016-2019 The Android Open Source Project
REM Copyright (C) 2015-2019 Valve Corporation
REM Copyright (C) 2015-2019 LunarG, Inc.
REM
REM Licensed under the Apache License, Version 2.0 (the "License");
REM you may not use this file except in compliance with the License.
REM You may obtain a copy of the License at
REM
REM      http://www.apache.org/licenses/LICENSE-2.0
REM
REM Unless required by applicable law or agreed to in writing, software
REM distributed under the License is distributed on an "AS IS" BASIS,
REM WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
REM See the License for the specific language governing permissions and
REM limitations under the License.
REM

setlocal EnableDelayedExpansion
set errorCode=0
set ANDROID_BUILD_DIR=%~dp0
set BUILD_DIR=%ANDROID_BUILD_DIR%
set BASE_DIR=%BUILD_DIR%\third_party
set JSONCPP_DIR=%BASE_DIR%\jsoncpp
set VULKAN_TOOLS_DIR=%BASE_DIR%\Vulkan-Tools
set VULKAN_HEADERS_DIR=%BASE_DIR%\Vulkan-Headers
set VULKAN_VALIDATIONLAYERS_DIR=%BASE_DIR%\Vulkan-ValidationLayers

for %%X in (where.exe) do (set FOUND=%%~$PATH:X)
if not defined FOUND (
   echo Dependency check failed:
   echo   where.exe not found
   echo   This script requires Windows Vista or later, which includes where.exe.
   set errorCode=1
)

where /q git.exe
if %ERRORLEVEL% equ 1 (
   echo Dependency check failed:
   echo   git.exe not found
   echo   Git for Windows can be downloaded here:  https://git-scm.com/download/win
   echo   Install and ensure git.exe makes it into your PATH
   set errorCode=1
)

where /q ndk-build.cmd
if %ERRORLEVEL% equ 1 (
   echo Dependency check failed:
   echo   ndk-build.cmd not found
   echo   Android NDK can be downloaded here:  http://developer.android.com/ndk/guides/setup.html
   echo   Install and ensure ndk-build.cmd makes it into your PATH
   set errorCode=1
)

REM ensure where is working with below false test
REM where /q foo
REM if %ERRORLEVEL% equ 1 (
REM echo foo
REM )

:main

if %errorCode% neq 0 (goto:error)

REM Read the target versions from external file, which is shared with Linux script

if not exist %ANDROID_BUILD_DIR%\jsoncpp_revision_android (
   echo.
   echo Missing jsoncpp_revision_android file. Place it in %ANDROID_BUILD_DIR%
   set errorCode=1
   goto:error
)

if not exist %ANDROID_BUILD_DIR%\vulkan-tools_revision_android (
   echo.
   echo Missing vulkan-tools_revision_android file. Place it in %ANDROID_BUILD_DIR%
   set errorCode=1
   goto:error
)

if not exist %ANDROID_BUILD_DIR%\vulkan-validationlayers_revision_android (
   echo.
   echo Missing vulkan-validationlayers_revision_android file. Place it in %ANDROID_BUILD_DIR%
   set errorCode=1
   goto:error
)

set /p JSONCPP_REVISION= < jsoncpp_revision_android
set /p VULKAN_TOOLS_REVISION= < vulkan-tools_revision_android
set /p VULKAN_VALIDATIONLAYERS_REVISION= < vulkan-validationlayers_revision_android
set /p VULKAN_HEADERS_REVISION= < vulkan-headers_revision_android
echo JSONCPP_REVISION=%JSONCPP_REVISION%
echo VULKAN_TOOLS_REVISION=%VULKAN_TOOLS_REVISION%
echo VULKAN_VALIDATIONLAYERS_REVISION=%VULKAN_VALIDATIONLAYERS_REVISION%
echo VULKAN_HEADERS_REVISION=%VULKAN_HEADERS_REVISION%

echo Creating and/or updating jsoncpp, vulkan-tools, vulkan-validationlayers in %BASE_DIR%

set sync-vulkan-headers=1
set sync-jsoncpp=1
set sync-vulkan-tools=1
set sync-vulkan-validationlayers=1
set build-jsoncpp=1

REM Always init the submodules, which includes vulkan headers
echo Initializing submodules
git submodule update --init --recursive

if %sync-vulkan-headers% equ 1 (
   if %ERRORLEVEL% neq 0 (goto:error)
   if not exist %VULKAN_HEADERS_DIR% (
      call:create_vulkan-headers
   )
   if %errorCode% neq 0 (goto:error)
   call:update_vulkan-headers
   if %errorCode% neq 0 (goto:error)
)

if %sync-jsoncpp% equ 1 (
   if exist %JSONCPP_DIR% (
      rd /S /Q %JSONCPP_DIR%
   )
   if %errorlevel% neq 0 (goto:error)
   if not exist %JSONCPP_DIR% (
      call:create_jsoncpp
   )
   if %errorCode% neq 0 (goto:error)
   call:update_jsoncpp
   if %errorCode% neq 0 (goto:error)
)

if %sync-vulkan-tools% equ 1 (
   if %ERRORLEVEL% neq 0 (goto:error)
   if not exist %VULKAN_TOOLS_DIR% (
      call:create_vulkan-tools
   )
   if %errorCode% neq 0 (goto:error)
   call:update_vulkan-tools
   if %errorCode% neq 0 (goto:error)
)

if %sync-vulkan-validationlayers% equ 1 (
   if %ERRORLEVEL% neq 0 (goto:error)
   if not exist %VULKAN_VALIDATIONLAYERS_DIR% (
      call:create_vulkan-validationlayers
   )
   if %errorCode% neq 0 (goto:error)
   call:update_vulkan-validationlayers
   if %errorCode% neq 0 (goto:error)
)

if %build-jsoncpp% equ 1 (
   call:build_jsoncpp
   if %errorCode% neq 0 (goto:error)
)


echo.
echo Exiting
goto:finish

:error
echo.
echo Halting due to error
goto:finish

:finish
if not "%cd%\" == "%BUILD_DIR%" ( cd %BUILD_DIR% )
endlocal
REM This needs a fix to return error, something like exit %errorCode%
REM Right now it is returning 0
goto:eof



REM // ======== Functions ======== //

:create_vulkan-headers
    echo.
    echo Creating local vulkan-headers repository %VULKAN_HEADERS_DIR%
    if not exist "%VULKAN_HEADERS_DIR%\" mkdir %VULKAN_HEADERS_DIR%
    cd %VULKAN_HEADERS_DIR%
    git clone https://github.com/KhronosGroup/Vulkan-Headers.git .
    git checkout %VULKAN_HEADERS_REVISION%
    if not exist %VULKAN_HEADERS_DIR%\registry (
       echo vulkan-headers source download failed!
       set errorCode=1
    )
goto:eof
 
:update_vulkan-headers
    echo.
    echo Updating %VULKAN_HEADERS_DIR%
    cd %VULKAN_HEADERS_DIR%
    git fetch --all
    git checkout %VULKAN_HEADERS_REVISION%
    if not exist %VULKAN_HEADERS_DIR%\registry (
       echo vulkan-headers source update failed!
       set errorCode=1
    )
goto:eof
 
:create_jsoncpp
   echo.
   echo Creating local jsoncpp repository %JSONCPP_DIR%)
   if not exist "%JSONCPP_DIR%\" mkdir %JSONCPP_DIR%
   cd %JSONCPP_DIR%
   git clone https://github.com/open-source-parsers/jsoncpp.git .
   git checkout %JSONCPP_REVISION%
   if not exist %JSONCPP_DIR%\include\json\json.h (
      echo jsoncpp source download failed!
      set errorCode=1
   )
goto:eof

:update_jsoncpp
   echo.
   echo Updating %JSONCPP_DIR%
   cd %JSONCPP_DIR%
   git fetch --all
   git checkout %JSONCPP_REVISION%
goto:eof

:build_jsoncpp
   echo.
   echo Building %JSONCPP_DIR%
   cd  %JSONCPP_DIR%
   python amalgamate.py

   if not exist %JSONCPP_DIR%\dist\json\json.h (
      echo.
      echo JsonCPP Amalgamation failed to generate %JSONCPP_DIR%\dist\json\json.h
      set errorCode=1
   )
goto:eof

:create_vulkan-tools
   echo.
   echo Creating local vulkan-tools repository %VULKAN_TOOLS_DIR%
   if not exist "%VULKAN_TOOLS_DIR%\" mkdir %VULKAN_TOOLS_DIR%
   cd %VULKAN_TOOLS_DIR%
   git clone https://github.com/KhronosGroup/Vulkan-Tools.git .
   git checkout %VULKAN_TOOLS_REVISION%
   if not exist %VULKAN_TOOLS_DIR%\common (
      echo vulkan-tools source download failed!
      set errorCode=1
   )
goto:eof

:update_vulkan-tools
   echo.
   echo Updating %VULKAN_TOOLS_DIR%
   cd %VULKAN_TOOLS_DIR%
   git fetch --all
   git checkout %VULKAN_TOOLS_REVISION%
   if not exist %VULKAN_TOOLS_DIR%\common (
      echo vulkan-tools source update failed!
      set errorCode=1
   )
goto:eof

:create_vulkan-validationlayers
   echo.
   echo Creating local vulkan-validationlayers repository %VULKAN_VALIDATIONLAYERS_DIR%
   if not exist "%VULKAN_VALIDATIONLAYERS_DIR%\" mkdir %VULKAN_VALIDATIONLAYERS_DIR%
   cd %VULKAN_VALIDATIONLAYERS_DIR%
   git clone https://github.com/KhronosGroup/Vulkan-ValidationLayers.git .
   git checkout %VULKAN_VALIDATIONLAYERS_REVISION%
   if not exist %VULKAN_VALIDATIONLAYERS_DIR% (
      echo vulkan-validationlayers source download failed!
      set errorCode=1
   )
goto:eof

:update_vulkan-validationlayers
   echo.
   echo Updating %VULKAN_VALIDATIONLAYERS_DIR%
   cd %VULKAN_VALIDATIONLAYERS_DIR%
   git fetch --all
   git checkout %VULKAN_VALIDATIONLAYERS_REVISION%
   if not exist %VULKAN_VALIDATIONLAYERS_DIR% (
      echo vulkan-validationlayers source update failed!
      set errorCode=1
   )
goto:eof
