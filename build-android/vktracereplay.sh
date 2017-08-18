#!/bin/bash

#set -vex

script_start_time=$(date +%s)

default_vkreplay_apk=./vkreplay/bin/NativeActivity-debug.apk
default_vktrace_exe=../build/vktrace/vktrace
default_vktrace32_exe=../build32/vktrace/vktrace32
default_activity=android.app.NativeActivity
default_frame=1

#if [[ $(basename "$PWD") != "build-android" ]]
#then
#    echo "Please run this script from the build-android directory"
#    exit 1
#fi

#
# Parse parameters
#

function printUsage {
   echo "Supported parameters are:"
   echo "    --serial <target device serial number>"
   echo "    --abi <abi to install>"
   echo "    --vktrace <full path to vktrace on host> (optional)"
   echo "    --vkreplay <full path to vkreplay APK> (optional)"
   echo "    --apk <full path to APK>"
   echo "    --package <package name>"
   echo "    --actvity <launchable-activity name>"
   echo "    --frame <frame number to capture>"
   echo
   echo "i.e. ${0##*/} --serial 01234567 \\"
   echo "              --abi arm64-v8a \\"
   echo "              --vktrace ../build/vktrace/vktrace \\"
   echo "              --vkreplay ./vkreplay/bin/NativeActivity-debug.apk \\"
   echo "              --apk ~/Downloads/foo.apk.apk \\"
   echo "              --package com.example.foo \\"
   echo "              --actvity android.app.NativeActivity \\"
   echo "              --frame 1"
   exit 1
}

if [[ $(($# % 2)) -ne 0 ]]
then
    echo Parameters must be provided in pairs.
    echo parameter count = $#
    echo
    printUsage
    exit 1
fi

while [[ $# -gt 0 ]]
do
    case $1 in
        --serial)
            # include the flag, because we need to leave it off if not provided
            serial="$2"
            serialFlag="-s $serial"
            shift 2
            ;;
        --abi)
            target_abi="$2"
            shift 2
            ;;
        --vktrace)
            vktrace_exe="$2"
            shift 2
            ;;
        --vkreplay)
            vkreplay_apk="$2"
            shift 2
            ;;
        --apk)
            apk="$2"
            shift 2
            ;;
        --package)
            package="$2"
            shift 2
            ;;
        --activity)
            activity="$2"
            shift 2
            ;;
        --frame)
            frame="$2"
            shift 2
            ;;
        -*)
            # unknown option
            echo Unknown option: $1
            echo
            printUsage
            exit 1
            ;;
    esac
done

echo serial = $serial

if [[ -z $serial ]]
then
    echo Please provide a serial number.
    echo
    printUsage
    exit 1
fi

if [[ $(adb devices) != *"$serial"* ]];
then
    echo Device not found: $serial
    echo
    printUsage
    exit 1
fi

default_target_abi=$(adb $serialFlag shell getprop ro.product.cpu.abi)
if [[ -z $target_abi ]];
then
    echo Using default target_abi
    target_abi=$default_target_abi
fi
echo target_abi = $target_abi

if [[ $target_abi == "armeabi-v7a" ]] ||
   [[ $target_abi == "mips" ]] ||
   [[ $target_abi == "x86" ]];
then
    echo Targeting 32-bit abi $target_abi
    target_32bit_abi=1
fi

if [[ -z $vktrace_exe ]];
then
    echo Using default vktrace_exe
    vktrace_exe=$default_vktrace_exe
    if [[ $target_32bit_abi ]]
    then
        vktrace_exe=$default_vktrace32_exe
    fi
else
    if [[ $target_32bit_abi ]]
    then
       echo Ensure your vktrace is 32-bit, i.e. vktrace32
    fi
fi
echo vktrace_exe = $vktrace_exe

if [[ -z $vkreplay_apk ]];
then
    echo Using default vkreplay_apk
    vkreplay_apk=$default_vkreplay_apk
fi
echo vkreplay_apk = $vkreplay_apk

if [[ -z $apk ]];
then
    echo target APK required
    exit 1
fi
echo apk = $apk

if [[ -z $package ]];
then
    echo target package name required
    exit 1
fi
echo package = $package

if [[ -z $activity ]];
then
    echo Using default activity
    activity=$default_activity
fi
echo activity = $activity

if [[ -z $frame ]];
then
    echo Using default screenshot frame
    frame=$default_frame
fi
echo frame = $frame

function printLayerBuild() {
    echo "To build layers:"
    echo "./update_external_sources_android.sh"
    echo "./android-generate.sh"
    echo "ndk-build -j"
}

function printvkreplayBuild() {
    echo "To build vkreplay apk"
    echo "android update project -s -p . -t \"android-23\""
    echo "ndk-build -j"
    echo "ant -buildfile vkreplay debug"
}

function printvktraceBuild() {
    echo "To build vktrace"
    echo "pushd .."
    echo "mkdir build"
    echo "cd build"
    echo "cmake -DCMAKE_BUILD_TYPE=Debug .."
    echo "make -j"
}

#
# If any parameter not found, print how to build it
#

if [ ! -f $vkreplay_apk ]; then
    echo "$vkreplay_apk not found!"
    printvkreplayBuild
    exit 1
fi

if [ ! -f $vktrace_exe ]; then
    echo "$vktrace_exe not found!"
    printvktraceBuild
    exit 1
fi

#
# Check for required tools
#

adb_path=$(which adb)
if [[ $? == 0 ]];
then
    echo using $adb_path
else
    echo adb not found, exiting
    echo check your NDK for it and add to path
    exit 1
fi
aapt_path=$(which aapt)
if [[ $? == 0 ]];
then
    echo using $aapt_path
else
    echo aapt not found, exiting
    echo check your NDK for it and add to path
    exit 1
fi
jar_path=$(which jar)
if [[ $? == 0 ]];
then
    echo using $jar_path
else
    echo jar not found, exiting
    exit 1
fi

#
# Ensure APKs can be traced
#

apk_badging=$(aapt dump badging $apk)
if [[ $apk_badging != *"uses-permission: name='android.permission.READ_EXTERNAL_STORAGE'"* ]] ||
   [[ $apk_badging != *"uses-permission: name='android.permission.WRITE_EXTERNAL_STORAGE'"* ]];
then
    echo Please package APK with the following permissions:
    echo     android.permission.READ_EXTERNAL_STORAGE
    echo     android.permission.WRITE_EXTERNAL_STORAGE
    exit 1
fi
apk_contents=$(jar tvf $apk)
if [[ $apk_contents != *"libVkLayer_screenshot.so"* ]] ||
   [[ $apk_contents != *"libVkLayer_vktrace_layer.so"* ]];
then
    echo Your APK does not contain the following layers:
    echo     libVkLayer_screenshot.so
    echo     libVkLayer_vktrace_layer.so
    echo You\'ll need to provide them another way.
    echo Continuing...
fi

#
# Start up
#

# We want to halt on errors here
set -e

# Wake up the device
adb $serialFlag shell input keyevent "KEYCODE_MENU"
adb $serialFlag shell input keyevent "KEYCODE_HOME"

# clean up anything lingering from previous runs
adb $serialFlag shell am force-stop $package
adb $serialFlag shell am force-stop com.example.vkreplay
if [[ $(adb $serialFlag shell pm list packages $package) == "package:$package" ]]
then
    adb $serialFlag uninstall $package && echo continuing...
fi
if [[ $(adb $serialFlag shell pm list packages com.example.vkreplay) == "package:com.example.vkreplay" ]]
then
    adb $serialFlag uninstall com.example.vkreplay && echo continuing...
fi
adb $serialFlag shell rm -f /sdcard/Android/$frame.ppm
adb $serialFlag shell rm -f /sdcard/Android/$package.$frame.vktrace.ppm
adb $serialFlag shell rm -f /sdcard/Android/$package.$frame.vkreplay.ppm
rm -f $package.$frame.vktrace.ppm
rm -f $package.$frame.vkreplay.ppm
rm -f $package.vktrace
rm -f $package\0.vktrace

# Ensure vktrace wasn't already running
let "script_run_time=$(date +%s)-$script_start_time"
killall -9 --older-than "$script_run_time"s vktrace || echo No vktrace found, continuing...
killall -9 --older-than "$script_run_time"s vktrace32 || echo No vktrace32 found, continuing...

# install the latest APK, possibly packaged with vktrace and screenshot
adb $serialFlag install --abi $target_abi $apk

# install vkreplay APK
adb $serialFlag install --abi $target_abi $vkreplay_apk

# trace and screenshot
adb $serialFlag shell setprop debug.vulkan.layer.1 VK_LAYER_LUNARG_vktrace
adb $serialFlag shell setprop debug.vulkan.layer.2 VK_LAYER_LUNARG_screenshot
adb $serialFlag shell pm grant $package android.permission.READ_EXTERNAL_STORAGE
adb $serialFlag shell pm grant $package android.permission.WRITE_EXTERNAL_STORAGE
adb $serialFlag shell setprop debug.vulkan.screenshot $frame

# vktrace
adb $serialFlag reverse localabstract:vktrace tcp:34201
$vktrace_exe -v full -o $package.vktrace &
adb $serialFlag shell am start $package/$activity

# don't halt on error for this loop
set +e

# wait until trace screenshot arrives, or a timeout
vktrace_seconds=300                                    # Duration in seconds.
vktrace_end_time=$(( $(date +%s) + vktrace_seconds ))  # Calculate end time.
sleep 5 # pause to let the screenshot write finish
until adb $serialFlag shell ls -la /sdcard/Android/$frame.ppm
do
    echo "Waiting for $package.vktrace screenshot on $serial"

    if [ $(date +%s) -gt $vktrace_end_time ]
    then
        echo "vktrace timeout reached: $vktrace_seconds seconds"
        echo "No vktrace screenshot, closing down"
        exit 1
    fi

    sleep 5
done

# stop our background vktrace
kill -9 $!

# pause for a moment to let our trace file finish writing
sleep 1

# halt on errors here
set -e

# set up for vkreplay
adb $serialFlag shell am force-stop $package
if [ -f $package.vktrace ]; then
    adb $serialFlag push $package.vktrace /sdcard/$package.vktrace
fi
if [ -f $package\0.vktrace ]; then
    adb $serialFlag push $package\0.vktrace /sdcard/$package.vktrace
fi

# grab the screenshot
adb $serialFlag pull /sdcard/Android/$frame.ppm $package.$frame.vktrace.ppm
adb $serialFlag shell mv /sdcard/Android/$frame.ppm /sdcard/Android/$package.$frame.vktrace.ppm

# replay and screenshot
adb $serialFlag shell setprop debug.vulkan.layer.1 VK_LAYER_LUNARG_screenshot
adb $serialFlag shell setprop debug.vulkan.layer.2 '""'
adb $serialFlag shell setprop debug.vulkan.screenshot $frame
adb $serialFlag shell pm grant com.example.vkreplay android.permission.READ_EXTERNAL_STORAGE
adb $serialFlag shell pm grant com.example.vkreplay android.permission.WRITE_EXTERNAL_STORAGE
sleep 5 # small pause to allow permission to take

# Wake up the device
adb $serialFlag shell input keyevent "KEYCODE_MENU"
adb $serialFlag shell input keyevent "KEYCODE_HOME"

adb $serialFlag shell am start -a android.intent.action.MAIN -c android-intent.category.LAUNCH -n com.example.vkreplay/android.app.NativeActivity --es args "-v\ full\ -t\ /sdcard/$package.vktrace"

# don't halt on the errors in this loop
set +e

# wait until vkreplay screenshot arrives, or a timeout
vkreplay_seconds=300                                     # Duration in seconds.
vkreplay_end_time=$(( $(date +%s) + vkreplay_seconds ))  # Calculate end time.
sleep 5 # pause to let the screenshot write finish
until adb $serialFlag shell ls -la /sdcard/Android/$frame.ppm
do
    echo "Waiting for vkreplay screenshot on $serial"

    if [ $(date +%s) -gt $vkreplay_end_time ]
    then
        echo "vkreplay timeout reached: $vkreplay_seconds seconds"
        echo "No vkreplay screenshot, closing down"
        exit 1
    fi
    sleep 5
done

# halt on any errors here
set -e

# grab the screenshot
adb $serialFlag pull /sdcard/Android/$frame.ppm $package.$frame.vkreplay.ppm
adb $serialFlag shell mv /sdcard/Android/$frame.ppm /sdcard/Android/$package.$frame.vkreplay.ppm

# clean up
adb $serialFlag shell am force-stop com.example.vkreplay
adb $serialFlag shell setprop debug.vulkan.layer.1 '""'

# don't halt in the exit code below
set +e

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

cmp -s $package.$frame.vktrace.ppm $package.$frame.vkreplay.ppm

if [ $? -eq 0 ] ; then
    printf "$GREEN[  PASSED  ]$NC {$apk-$package}\n"
else
    printf "$RED[  FAILED  ]$NC screenshot file compare failed\n"
    printf "$RED[  FAILED  ]$NC {$apk-$package}\n"
    printf "TEST FAILED\n"
    exit 1
fi

exit 0
