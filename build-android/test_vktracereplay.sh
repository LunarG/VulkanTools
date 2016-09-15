#!/bin/bash

#set -v

adb root
adb shell setenforce 0
adb shell rm -rf /data/local/debug/vulkan
adb shell mkdir -p /data/local/debug/vulkan
adb push libs/arm64-v8a/libVkLayer_vktrace_layer.so /data/local/debug/vulkan/
adb push libs/arm64-v8a/libVkLayer_screenshot.so /data/local/debug/vulkan/

# clean up anything lingering from previous runs
adb shell am force-stop com.example.Cube
adb shell am force-stop com.example.vkreplay

# build and install Cube with INTERNET permission

# trace and screenshot
adb shell setprop debug.vulkan.layer.1 VK_LAYER_LUNARG_vktrace
adb shell setprop debug.vulkan.layer.2 VK_LAYER_LUNARG_screenshot
adb shell pm grant com.example.Cube android.permission.READ_EXTERNAL_STORAGE
adb shell pm grant com.example.Cube android.permission.WRITE_EXTERNAL_STORAGE
adb shell setprop debug.vulkan.screenshot 1

# vktrace
adb reverse tcp:34201 tcp:34201
../build/vktrace/vktrace -v full -o cube.vktrace &

# vktrace layer
adb shell am start com.example.Cube/android.app.NativeActivity
# hack hack hack - this should wait until the screenshot arrives, or until a timeout
sleep 10
kill $!
adb shell am force-stop com.example.Cube
adb push cube0.vktrace /sdcard/cube0.vktrace

# grab the screenshot
adb pull /sdcard/Android/1.ppm cube.ppm
adb shell mv /sdcard/Android/1.ppm /sdcard/Android/cube.ppm

# vkreplay
android update project -s -p . -t "android-23"
ant -buildfile vkreplay debug
adb uninstall com.example.vkreplay
adb install ./vkreplay/bin/NativeActivity-debug.apk

# replay and screenshot
adb shell setprop debug.vulkan.layer.1 VK_LAYER_LUNARG_screenshot
adb shell setprop debug.vulkan.layer.2 '""'
adb shell pm grant com.example.vkreplay android.permission.READ_EXTERNAL_STORAGE
adb shell pm grant com.example.vkreplay android.permission.WRITE_EXTERNAL_STORAGE

adb shell am start -a android.intent.action.MAIN -c android-intent.category.LAUNCH -n com.example.vkreplay/android.app.NativeActivity --es args "-v\ full\ -t\ /sdcard/cube0.vktrace"

# grab the screenshot
# hack hack hack - this should wait until the screenshot arrives, or until a timeout
sleep 10
adb pull /sdcard/Android/1.ppm vkreplay.ppm

adb shell am force-stop com.example.vkreplay

# the rest is a quick port from vktracereplay.sh

if [ -t 1 ] ; then
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    NC='\033[0m' # No Color
else
    RED=''
    GREEN=''
    NC=''
fi

cmp -s cube.ppm vkreplay.ppm

PGM=Cube

if [ $? -eq 0 ] ; then
    printf "$GREEN[  PASSED  ]$NC ${PGM}\n"
else
    printf "$RED[  FAILED  ]$NC screenshot file compare failed\n"
    printf "$RED[  FAILED  ]$NC ${PGM}\n"
    printf "TEST FAILED\n"
    exit 1
fi

exit 0
