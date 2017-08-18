#!/bin/bash

#set -vex

script_start_time=$(date +%s)

default_vktrace_exe=../build/vktrace/vktrace
default_vktrace32_exe=../build32/vktrace/vktrace32
default_activity=android.app.NativeActivity

#
# Parse parameters
#

function printUsage {
   echo "Supported parameters are:"
   echo "    --serial <target device serial number>"
   echo "    --abi <abi to install>"
   echo "    --vktrace <full path to vktrace on host> (optional)"
   echo "    --package <package name>"
   echo "    --actvity <launchable-activity name>"
   echo "    --frame <frame number to capture> (optional)"
   echo
   echo "i.e. ${0##*/} --serial 01234567 \\"
   echo "              --abi arm64-v8a \\"
   echo "              --vktrace ../build/vktrace/vktrace \\"
   echo "              --package com.example.foo \\"
   echo "              --actvity android.app.NativeActivity \\"
   echo "              --frame 100"
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
    echo Not attempting to record any screenshot
else
    echo Attempting to record screenshot of frame $frame
fi

function printLayerBuild() {
    echo "To build layers:"
    echo "./update_external_sources_android.sh"
    echo "./android-generate.sh"
    echo "ndk-build -j"
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

# Ensure vktrace wasn't already running
let "script_run_time=$(date +%s)-$script_start_time"
killall --older-than "$script_run_time"s vktrace || echo continuing...

# Enable trace layer
adb $serialFlag shell setprop debug.vulkan.layer.1 VK_LAYER_LUNARG_vktrace

# Start vktrace
adb $serialFlag reverse localabstract:vktrace tcp:34201
sleep 1
$vktrace_exe -v full -o $package.vktrace &

# Start the target activity
adb $serialFlag shell am start $package/$activity

# wait for a keystroke, indicating when tracing should stop
read -rsn1

# stop our background vktrace
kill $!

# clean up
adb $serialFlag shell am force-stop $package
adb $serialFlag shell setprop debug.vulkan.layer.1 '""'

exit 0
