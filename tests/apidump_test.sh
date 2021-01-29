#!/bin/bash

# apidump_test.sh
# This script will run the demo vulkaninfo with the api_dump layer and capture the output.
# The script will search the output for a certain APIs that should appear in the output.
# If the threshold is met, the test will indicate PASS, else FAILURE. This
# script requires a path to the Vulkan-Tools build directory so that it can locate
# vulkaninfo and the mock ICD. The path can be defined using the environment variable
# VULKAN_TOOLS_BUILD_DIR or using the command-line argument -t or --tools.

# Track unrecognized arguments.
UNRECOGNIZED=()

# Parse the command-line arguments.
while [[ $# -gt 0 ]]
do
   KEY="$1"
   case $KEY in
      -t|--tools)
      VULKAN_TOOLS_BUILD_DIR="$2"
      shift
      shift
      ;;
      *)
      UNRECOGNIZED+=("$1")
      shift
      ;;
   esac
done

# Reject unrecognized arguments.
if [[ ${#UNRECOGNIZED[@]} -ne 0 ]]; then
   echo "ERROR: $0:$LINENO"
   ehco "Unrecognized command-line arguments: ${UNRECOGNIZED[*]}"
   exit 1
fi

if [ -z ${VULKAN_TOOLS_BUILD_DIR+x} ]; then
   echo "ERROR: $0:$LINENO"
   echo "Vulkan-Tools build directory is undefined."
   echo "Please set VULKAN_TOOLS_BUILD_DIR or use the -t|--tools <path> command line option."
   exit 1
fi

if [ -t 1 ] ; then
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    NC='\033[0m' # No Color
else
    RED=''
    GREEN=''
    NC=''
fi

pushd $(dirname "${BASH_SOURCE[0]}")

VULKANINFO="$VULKAN_TOOLS_BUILD_DIR/install/bin/vulkaninfo"
VK_ICD_FILENAMES="$VULKAN_TOOLS_BUILD_DIR/icd/VkICD_mock_icd.json" \
    VK_INSTANCE_LAYERS=VK_LAYER_LUNARG_api_dump \
    "$VULKANINFO" --show-formats > apidump_file.tmp

printf "$GREEN[ RUN      ]$NC $0\n"
#echo DISPLAY=$DISPLAY         # Debug
#ls -l apidump_file.tmp        # Debug
if [ -f apidump_file.tmp ]
then
    #echo @===================@   # Debug
    #cat apidump_file.tmp         # Debug
    #echo @===================@   # Debug
    GPDFP_count=$(grep vkGetPhysicalDeviceFormatProperties apidump_file.tmp | wc -l)
    pipelineCacheUUID_count=$(grep pipelineCacheUUID apidump_file.tmp | wc -l)
    vk_format_feature_count=$(grep VK_FORMAT_FEATURE apidump_file.tmp | wc -l)
    echo COUNTS: $GPDFP_count $pipelineCacheUUID_count $vk_format_feature_count  # Debug
    if (( $GPDFP_count > 50 && $pipelineCacheUUID_count > 10 && $vk_format_feature_count > 500 ))
    then
        printf "$GREEN[  PASSED  ]$NC $0\n"
    else
        printf "$RED[  FAILED  ]$NC $0\n"
        rm apidump_file.tmp
        popd
        exit 1
    fi
fi

rm apidump_file.tmp
popd

exit 0
