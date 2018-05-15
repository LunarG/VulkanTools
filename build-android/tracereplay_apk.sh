function printUsage {
   echo "Supported parameters are:"
   echo "    -p|--platform <platform> (optional)"
   echo "    -s|--serial <target device serial number> (optional)"
   echo "    -a|--apk <target apk file>"
   echo "    -k|--package <target apk package>"
   echo "    -v|--vktrace <vktrace executable path>"
   echo
   echo "i.e. ${0##*/} -p <platform> -f <test filter> -s <serial number>"
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
        -p|--platform)
            platform="$2"
            shift 2
            ;;
        -s|--serial)
            serial="$2"
            shift 2
            ;;
        -a|--apk)
            apk="$2"
            shift 2
            ;;
        -k|--package)
            package="$2"
            shift 2
            ;;
        -v|--vktrace)
            vktrace="$2"
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

if [[ $serial ]]; then
    serialFlag="-s $serial"
    if [[ $(adb devices) != *"$serial"* ]]
    then
        echo Device not found: "${serial}"
        echo
        printUsage
        exit 1
    fi
else
    serial=$(adb get-serialno)
    echo Using device $serial
fi

if [[ -z $platform ]]
then
    echo No platform specified.
    printUsage
    exit 1
fi

if [[ -z $apk ]]
then
    echo No apk specified.
    printUsage
    exit 1
fi

if [[ -z $package ]]
then
    echo No package specified.
    printUsage
    exit 1
fi

if [[ -z $vktrace ]]
then
    echo No vktrace specified.
    printUsage
    exit 1
fi

if [[ $platform ]]; then echo platform = "${platform}"; fi
if [[ $serial ]]; then echo serial = "${serial}"; fi

TMP_FOLDER=$PWD/tmp_vktracereplay_android
mkdir $TMP_FOLDER

cp $apk $TMP_FOLDER/temp-unaligned.apk

# Add Trace and Screenshot layer object files to apk copy

pushd ./bin/libs # Need the correct relative path (../lib/platform/layer.so) so that aapt creates the right folder structure in the apk
aapt add $TMP_FOLDER/temp-unaligned.apk lib/$platform/libVkLayer_vktrace_layer.so lib/$platform/libVkLayer_screenshot.so
popd

cd $TMP_FOLDER

# Sign apk
jarsigner -verbose -keystore ~/.android/debug.keystore -storepass android -keypass android ./temp-unaligned.apk androiddebugkey

# Align apk
zipalign -f 4 ./temp-unaligned.apk ./temp.apk

# Trace and replay
../vktracereplay.sh \
    --serial $serial \
    --abi $platform \
    --vktrace $vktrace \
    --vkreplay ../vkreplay/bin/vkreplay.apk \
    --apk ./temp.apk \
    --package $package \
    --activity android.app.NativeActivity \
    --frame 1

cd ..
rm $TMP_FOLDER -rf