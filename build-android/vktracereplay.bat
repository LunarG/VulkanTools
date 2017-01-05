@echo off
REM # Copyright 2016 The Android Open Source Project
REM # Copyright (C) 2016 Valve Corporation
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

setlocal EnableDelayedExpansion

set vktrace_exe=..\build\vktrace\Debug\vktrace.exe
set vktrace32_exe=..\build32\vktrace\Debug\vktrace.exe
set vkreplay_apk=.\vkreplay\bin\NativeActivity-debug.apk
set activity=android.app.NativeActivity
set frame=1
for /f "delims=" %%i in ('adb shell getprop ro.product.cpu.abi') do set target_abi=%%i

REM // ======== Parameter parsing ======== //

   if "%1" == "" (
       echo Supported parameters are:
       echo     --serial ^<target device serial number^>
       echo     --abi ^<abi to install^>
       echo     --vktrace ^<full path to vktrace on host^> ^(optional^)
       echo     --vkreplay ^<full path to vkreplay APK^> ^(optional^)
       echo     --apk ^<full path to APK^>
       echo     --package ^<package name^>
       echo     --actvity ^<launchable-activity name^>
       echo     --frame ^<frame number to capture^>
       echo. 
       echo i.e. %~nx0 --serial 01234567
       echo                        --abi arm64-v8a
       echo                        --vktrace ../build/vktrace/vktrace
       echo                        --vkreplay ./vkreplay/bin/NativeActivity-debug.apk
       echo                        --apk ~/Downloads/foo.apk.apk
       echo                        --package com.example.foo
       echo                        --actvity android.app.NativeActivity
       echo                        --frame 1
      goto:finish
   )

   REM Check for even parameter count
   set arg_count=0
   for %%x in (%*) do set /A arg_count+=1
   set /A even_args=arg_count%2
   if %even_args% neq 0 (
       echo Arguments must be provided in pairs.
       goto:finish
   )
   
   :parameterLoop

      if "%1"=="" goto:parameterContinue

      if "%1" == "--serial" (
         set serial=%2
         shift
         shift
         goto:parameterLoop
      )

      if "%1" == "--abi" (
         set target_abi=%2
         shift
         shift
         goto:parameterLoop
      )

      if "%1" == "--vktrace" (
         set vktrace_exe=%2
         shift
         shift
         goto:parameterLoop
      )

      if "%1" == "--vkreplay" (
         set vkreplay_apk=%2
         shift
         shift
         goto:parameterLoop
      )

      if "%1" == "--apk" (
         set apk=%2
         shift
         shift
         goto:parameterLoop
      )

      if "%1" == "--package" (
         set package=%2
         shift
         shift
         goto:parameterLoop
      )

      if "%1" == "--activity" (
         set activity=%2
         shift
         shift
         goto:parameterLoop
      )

      if "%1" == "--frame" (
         set frame=%2
         shift
         shift
         goto:parameterLoop
      )

      echo Unrecognized options "%1"
      goto:error

   :parameterContinue

REM // ======== end Parameter parsing ======== //

REM Match 32-bit and 64-bit ABIs between host and target
if "%target_abi%" == "armeabi-v7a" ( set vktrace_exe=%vktrace32_exe% )
if "%target_abi%" == "x86"         ( set vktrace_exe=%vktrace32_exe% )
if "%target_abi%" == "mips"        ( set vktrace_exe=%vktrace32_exe% )


echo serial = %serial%
echo target_abi = %target_abi%
echo vktrace_exe = %vktrace_exe%
echo vkreplay_apk = %vkreplay_apk%
echo apk = %apk%
echo package = %package%
echo activity = %activity%
echo frame = %frame%


REM Wake up the device
adb -s %serial% shell input keyevent "KEYCODE_MENU"
adb -s %serial% shell input keyevent "KEYCODE_HOME"

REM clean up anything lingering from previous runs
adb -s %serial% shell am force-stop %package%
adb -s %serial% shell am force-stop com.example.vkreplay

adb -s %serial% uninstall %package% && echo continuing...
adb -s %serial% uninstall com.example.vkreplay && echo continuing...

adb -s %serial% shell rm -f /sdcard/Android/%frame%.ppm
adb -s %serial% shell rm -f /sdcard/Android/%package%.%frame%.vktrace.ppm
adb -s %serial% shell rm -f /sdcard/Android/%package%.%frame%.vkreplay.ppm
del /Q /F %package%.%frame%.vktrace.ppm
del /Q /F %package%.%frame%.vkreplay.ppm

REM install the latest APK, possibly packaged with vktrace and screenshot
adb -s %serial% install --abi %target_abi% %apk%

REM install vkreplay APK
adb -s %serial% install --abi %target_abi% %vkreplay_apk%

REM trace and screenshot
adb -s %serial% shell setprop debug.vulkan.layer.1 VK_LAYER_LUNARG_vktrace
adb -s %serial% shell setprop debug.vulkan.layer.2 VK_LAYER_LUNARG_screenshot
adb -s %serial% shell pm grant %package% android.permission.READ_EXTERNAL_STORAGE
adb -s %serial% shell pm grant %package% android.permission.WRITE_EXTERNAL_STORAGE
adb -s %serial% shell setprop debug.vulkan.screenshot %frame%

REM vktrace
adb -s %serial% reverse localabstract:vktrace tcp:34201
start /b %vktrace_exe% -v full -o %package%.vktrace
adb -s %serial% shell am start %package%/%activity%


REM WAIT HERE FOR SCREENSHOT
REM TODO THIS SHOULD BE A LOOP
timeout /t 5

taskkill /f /im vktrace.exe
taskkill /f /im vktrace32.exe

REM set up for vkreplay
adb -s %serial% shell am force-stop %package%
adb -s %serial% push %package%0.vktrace /sdcard/%package%.vktrace

REM grab the screenshot
adb -s %serial% pull /sdcard/Android/%frame%.ppm %package%.%frame%.vktrace.ppm
adb -s %serial% shell mv /sdcard/Android/%frame%.ppm /sdcard/Android/%package%.%frame%.vktrace.ppm

REM replay and screenshot
adb -s %serial% shell setprop debug.vulkan.layer.1 VK_LAYER_LUNARG_screenshot
adb -s %serial% shell setprop debug.vulkan.layer.2 '""'
adb -s %serial% shell setprop debug.vulkan.screenshot %frame%
adb -s %serial% shell pm grant com.example.vkreplay android.permission.READ_EXTERNAL_STORAGE
adb -s %serial% shell pm grant com.example.vkreplay android.permission.WRITE_EXTERNAL_STORAGE
REM small pause to allow permission to take
timeout /t 5

REM Wake up the device
adb -s %serial% shell input keyevent "KEYCODE_MENU"
adb -s %serial% shell input keyevent "KEYCODE_HOME"

adb -s %serial% shell am start -a android.intent.action.MAIN -c android-intent.category.LAUNCH -n com.example.vkreplay/android.app.NativeActivity --es args "-v\ full\ -t\ /sdcard/%package%.vktrace"

REM WAIT HERE FOR SCREENSHOT
REM TODO THIS SHOULD BE A LOOP
timeout /t 5

REM grab the screenshot
adb -s %serial% pull /sdcard/Android/%frame%.ppm %package%.%frame%.vkreplay.ppm
adb -s %serial% shell mv /sdcard/Android/%frame%.ppm /sdcard/Android/%package%.%frame%.vkreplay.ppm

REM clean up
adb -s %serial% shell am force-stop com.example.vkreplay
adb -s %serial% shell setprop debug.vulkan.layer.1 '""'

REM compare screenshots
fc.exe /b %package%.%frame%.vktrace.ppm %package%.%frame%.vkreplay.ppm
 
if %errorlevel% neq 0 (
    echo Images DO NOT match - Failed
    exit /b 1
)

if %errorlevel% equ 0 (
    echo Images match - PASSED
    exit /b 0
)

:error
echo.
echo Halting due to error
goto:finish

:finish
endlocal
goto:eof
