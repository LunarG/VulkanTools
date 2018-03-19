#! /bin/bash
# Tests of LunarG Device Simulation (devsim) layer
# Uses 'jq' v1.5 https://stedolan.github.io/jq/

set -o nounset
set -o physical

cd $(dirname "${BASH_SOURCE[0]}")
LVL_BUILD_DIR="$PWD/../submodules/Vulkan-LoaderAndValidationLayers"

if [ -t 1 ] ; then
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    NC='\033[0m' # No Color
else
    RED=''
    GREEN=''
    NC=''
fi
printf "$GREEN[ RUN      ]$NC $0\n"

function fail_msg () {
   printf "$RED[  FAILED  ]$NC $1 failed\n"
   exit 1
}

export LD_LIBRARY_PATH="$LVL_BUILD_DIR/loader:${LD_LIBRARY_PATH:-}"
export VK_LAYER_PATH="$PWD/../layersvt"
export VK_INSTANCE_LAYERS="VK_LAYER_LUNARG_device_simulation"

#export VK_DEVSIM_DEBUG_ENABLE="1"
#export VK_DEVSIM_EXIT_ON_ERROR="1"
#export VK_LOADER_DEBUG="all"

#############################################################################
# Test 1: Load test config, compare vulkaninfo output against gold file.

FILENAME_01_TEMP1="devsim_test1_temp1.json"
FILENAME_01_TEMP2="devsim_test1_temp2.json"
rm -f $FILENAME_01_TEMP1 $FILENAME_01_TEMP2

export VK_DEVSIM_FILENAME="devsim_test1_in1.json:devsim_test1_in2.json:devsim_test1_in3.json:devsim_test1_in4.json:devsim_test1_in5.json"
"$LVL_BUILD_DIR/demos/vulkaninfo" -j > $FILENAME_01_TEMP1 2> /dev/null
[ $? -eq 0 ] || fail_msg "vulkaninfo"

JSON_SECTIONS='{VkPhysicalDeviceProperties,VkPhysicalDeviceFeatures,VkPhysicalDeviceMemoryProperties,ArrayOfVkQueueFamilyProperties,ArrayOfVkFormatProperties}'
jq -S $JSON_SECTIONS $FILENAME_01_TEMP1 > $FILENAME_01_TEMP2
[ $? -eq 0 ] || fail_msg "jq extraction"

jq --slurp  --exit-status '.[0] == .[1]' "devsim_test1_gold.json" $FILENAME_01_TEMP2 > /dev/null
[ $? -eq 0 ] || fail_msg "jq file compare"

#############################################################################

printf "$GREEN[  PASSED  ]$NC $0\n"
exit 0

# vim: set sw=4 ts=8 et ic ai:
