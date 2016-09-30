#!/bin/bash

set -v

script_start_time=$(date +%s)

default_vkreplay_apk=./vkreplay/bin/NativeActivity-debug.apk
default_vktrace_exe=../build/vktrace/vktrace
default_vktrace32_exe=../build/vktrace/vktrace32
default_cube_apk=../demos/android/cube-with-layers/bin/NativeActivity-debug.apk
default_target_abi=$(adb shell getprop ro.product.cpu.abi)

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
   echo "    -s|--serial <target device serial number>"
   echo "    -a|--abi <abi to install>"
   echo "    -t|--vktrace <full path to vktrace on host> (optional)"
   echo "    -r|--vkreplay <full path to vkreplay APK> (optional)"
   echo "    -c|--cube <full path to cube APK> (optional)"
   echo
   echo "i.e. ${0##*/} -s 01234567 \\"
   echo "              -a arm64-v8a \\"
   echo "              -t ../build/vktrace/vktrace \\"
   echo "              -r ./vkreplay/bin/NativeActivity-debug.apk \\"
   echo "              -c ../demos/android/cube-with-layers/bin/NativeActivity-debug.apk"
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
        -s|--serial)
            # include the flag, because we need to leave it off if not provided
            serial="$2"
            serialFlag="-s $serial"
            shift 2
            ;;
        -a|--abi)
            target_abi="$2"
            shift 2
            ;;
        -t|--vktrace)
            vktrace_exe="$2"
            shift 2
            ;;
        -r|--vkreplay)
            vkreplay_apk="$2"
            shift 2
            ;;
        -c|--cube)
            cube_apk="$2"
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

if [[ -z $cube_apk ]];
then
    echo Using default cube_apk
    cube_apk=$default_cube_apk
fi
echo cube_apk = $cube_apk

function printLayerBuild() {
    echo "To build layers:"
    echo "./update_external_sources_android.sh"
    echo "./android-generate.sh"
    echo "ndk-build -j"
}

function printCubeWithLayersBuild() {
    echo "To build CubeWithLayers apk:"
    echo "pushd ../demos/android"
    echo "android update project -s -p . -t \"android-23\""
    echo "ndk-build -j"
    echo "ant -buildfile cube-with-layers debug"
    echo "popd"
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
# If using default build, ensure layers have been built
#

if [[ "$cube_apk" == "$default_cube_apk" ]];
then
    vktracelayer_found=0
    for i in libs/*/libVkLayer_vktrace_layer.so;
        do test -f "$i" && let "vktracelayer_found++" && continue;
    done
    if [ "$vktracelayer_found" -le "0" ]; then
        echo "libVkLayer_vktrace_layer.so not found!"
        printLayerBuild
        exit 1
    fi

    screenshot_found=0
    for i in libs/*/libVkLayer_screenshot.so;
        do test -f "$i" && let "screenshot_found++" && continue;
    done
    if [ "$screenshot_found" -le "0" ]; then
        echo "libVkLayer_screenshot.so not found!"
        printLayerBuild
        exit 1
    fi
fi

if [[ "$vkreplay_apk" == "$default_vkreplay_apk" ]];
then
    vkreplay_found=0
    for i in libs/*/libvkreplay.so;
        do test -f "$i" && let "vkreplay_found++" && continue;
    done
    if [ "$vkreplay_found" -le "0" ]; then
        printLayerBuild
        echo "libvkreplay.so not found!"
        exit 1
    fi
fi

#
# If any parameter not found, print how to build it
#

if [ ! -f $cube_apk ]; then
    echo "$cube_apk not found!"
    printCubeWithLayersBuild
    exit 1
fi

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

cube_badging=$(aapt dump badging $cube_apk)
if [[ $cube_badging != *"uses-permission: name='android.permission.READ_EXTERNAL_STORAGE'"* ]] ||
   [[ $cube_badging != *"uses-permission: name='android.permission.WRITE_EXTERNAL_STORAGE'"* ]] ||
   [[ $cube_badging != *"uses-permission: name='android.permission.INTERNET'"* ]];
then
    echo Please package cube with the following permissions:
    echo     android.permission.READ_EXTERNAL_STORAGE
    echo     android.permission.WRITE_EXTERNAL_STORAGE
    echo     android.permission.INTERNAL_EXTERNAL_STORAGE
    exit 1
fi
apk_contents=$(jar tvf $cube_apk)
if [[ $apk_contents != *"libVkLayer_screenshot.so"* ]] ||
   [[ $apk_contents != *"libVkLayer_vktrace_layer.so"* ]];
then
    echo Please package cube with the following layers:
    echo     libVkLayer_screenshot.so
    echo     libVkLayer_vktrace_layer.so
    exit 1
fi

#
# Start up
#

# Wake up the device
adb $serialFlag shell input keyevent "KEYCODE_MENU"
adb $serialFlag shell input keyevent "KEYCODE_HOME"

# clean up anything lingering from previous runs
adb $serialFlag shell am force-stop com.example.CubeWithLayers
adb $serialFlag shell am force-stop com.example.vkreplay
if [[ $(adb $serialFlag shell pm list packages com.example.CubeWithLayers) == "package:com.example.CubeWithLayers" ]]
then
    adb $serialFlag uninstall com.example.CubeWithLayers && echo continuing...
fi
if [[ $(adb $serialFlag shell pm list packages com.example.vkreplay) == "package:com.example.vkreplay" ]]
then
    adb $serialFlag uninstall com.example.vkreplay && echo continuing...
fi
adb $serialFlat shell rm -f /sdcard/Android/cube.ppm
adb $serialFlat shell rm -f /sdcard/Android/vkreplay.ppm
rm -f cube.ppm
rm -f vkreplay.ppm

# Ensure vktrace wasn't already running
let "script_run_time=$(date +%s)-$script_start_time"
killall --older-than "$script_run_time"s vktrace || echo continuing...

# install the latest CubeWithLayers, packaged with vktrace and screenshot
adb $serialFlag install --abi $target_abi $cube_apk

# install vkreplay APK
adb $serialFlag install --abi $target_abi $vkreplay_apk

# trace and screenshot
adb $serialFlag shell setprop debug.vulkan.layer.1 VK_LAYER_LUNARG_vktrace
adb $serialFlag shell setprop debug.vulkan.layer.2 VK_LAYER_LUNARG_screenshot
adb $serialFlag shell pm grant com.example.CubeWithLayers android.permission.READ_EXTERNAL_STORAGE
adb $serialFlag shell pm grant com.example.CubeWithLayers android.permission.WRITE_EXTERNAL_STORAGE
adb $serialFlag shell setprop debug.vulkan.screenshot 1

# vktrace
adb $serialFlag reverse tcp:34201 tcp:34201
$vktrace_exe -v full -o cube.vktrace &
adb $serialFlag shell am start com.example.CubeWithLayers/android.app.NativeActivity

# wait until cube screenshot arrives, or a timeout
cube_seconds=30                                  # Duration in seconds.
cube_end_time=$(( $(date +%s) + cube_seconds ))  # Calculate end time.
sleep 5 # pause to let the screenshot write finish
until adb $serialFlag shell ls -la /sdcard/Android/1.ppm
do
    echo "Waiting for cube screenshot on $serial"

    if [ $(date +%s) -gt $cube_end_time ]
    then
        echo "Cube timeout reached: $cube_seconds seconds"
        echo "No cube screenshot, closing down"
        exit 1
    fi

    sleep 5
done

# stop our background vktrace
kill $!

# set up for vkreplay
adb $serialFlag shell am force-stop com.example.CubeWithLayers
adb $serialFlag push cube0.vktrace /sdcard/cube0.vktrace

# grab the screenshot
adb $serialFlag pull /sdcard/Android/1.ppm cube.ppm
adb $serialFlag shell mv /sdcard/Android/1.ppm /sdcard/Android/cube.ppm

# replay and screenshot
adb $serialFlag shell setprop debug.vulkan.layer.1 VK_LAYER_LUNARG_screenshot
adb $serialFlag shell setprop debug.vulkan.layer.2 '""'
adb $serialFlag shell setprop debug.vulkan.screenshot 1
adb $serialFlag shell pm grant com.example.vkreplay android.permission.READ_EXTERNAL_STORAGE
adb $serialFlag shell pm grant com.example.vkreplay android.permission.WRITE_EXTERNAL_STORAGE
sleep 5 # small pause to allow permission to take
adb $serialFlag shell am start -a android.intent.action.MAIN -c android-intent.category.LAUNCH -n com.example.vkreplay/android.app.NativeActivity --es args "-v\ full\ -t\ /sdcard/cube0.vktrace"

# wait until vkreplay screenshot arrives, or a timeout
vkreplay_seconds=30                                      # Duration in seconds.
vkreplay_end_time=$(( $(date +%s) + vkreplay_seconds ))  # Calculate end time.
sleep 5 # pause to let the screenshot write finish
until adb $serialFlag shell ls -la /sdcard/Android/1.ppm
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

# grab the screenshot
adb $serialFlag pull /sdcard/Android/1.ppm vkreplay.ppm
adb $serialFlag shell mv /sdcard/Android/1.ppm /sdcard/Android/vkreplay.ppm

# clean up
adb $serialFlag shell am force-stop com.example.vkreplay
adb $serialFlag shell setprop debug.vulkan.layer.1 '""'

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
