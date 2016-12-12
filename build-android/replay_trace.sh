#!/bin/bash

#
# Parse parameters
#

function printUsage {
   echo "Supported parameters are:"
   echo "    --serial <target device serial number>"
   echo "    --tracefile <path to tracefile>"
   echo
   echo "i.e. ${0##*/} --serial 01234567 \\"
   echo "              --tracefile cube.vktrace \\"
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
        --tracefile)
            tracefile="$2"
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

function printvkreplayBuild() {
    echo "To build vkreplay apk"
    echo "android update project -s -p . -t \"android-23\""
    echo "ndk-build -j"
    echo "ant -buildfile vkreplay debug"
}

#
# If any parameter not found, print how to build it
#

if [ ! -f $tracefile ]; then
    echo "$tracefile not found!"
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

#
# Start up
#

# We want to halt on errors here
set -e

# clean up anything lingering from previous runs
adb $serialFlag shell am force-stop com.example.vkreplay

# push the trace to the device
adb $serialFlag push $tracefile /sdcard/$tracefile

# replay and screenshot
adb $serialFlag shell pm grant com.example.vkreplay android.permission.READ_EXTERNAL_STORAGE
adb $serialFlag shell pm grant com.example.vkreplay android.permission.WRITE_EXTERNAL_STORAGE
sleep 1 # small pause to allow permission to take

# Wake up the device
adb $serialFlag shell input keyevent "KEYCODE_MENU"
adb $serialFlag shell input keyevent "KEYCODE_HOME"

# Start the replay
adb $serialFlag shell am start -a android.intent.action.MAIN -c android-intent.category.LAUNCH -n com.example.vkreplay/android.app.NativeActivity --es args "-v\ full\ -t\ /sdcard/$tracefile"

exit 0
