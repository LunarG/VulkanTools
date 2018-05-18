#! /bin/bash
# Tests of LunarG Device Simulation (devsim) layer
# Uses 'jq' v1.5 https://stedolan.github.io/jq/

set -o nounset
set -o physical

cd $(dirname "${BASH_SOURCE[0]}")
LVL_BUILD_DIR="$PWD/../submodules/Vulkan-ValidationLayers"

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
# Test 1: Verify devsim results using vkjson_info.

FILENAME_01_RESULT="device_simulation_layer_test_1.json"
FILENAME_01_STDOUT="device_simulation_layer_test_1.txt"
FILENAME_01_TEMP1="devsim_test1_temp1.json"
rm -f $FILENAME_01_RESULT $FILENAME_01_STDOUT $FILENAME_01_TEMP1

export VK_DEVSIM_FILENAME="devsim_test1_in_ArrayOfVkFormatProperties.json:devsim_test1_in.json"
"$LVL_BUILD_DIR/libs/vkjson/vkjson_info" > $FILENAME_01_STDOUT
[ $? -eq 0 ] || fail_msg "test1 vkjson_info"

# Use jq to extract, reformat, and sort the output.
jq -S '{properties,features,memory,queues,formats}' $FILENAME_01_RESULT > $FILENAME_01_TEMP1
[ $? -eq 0 ] || fail_msg "test1 jq extraction"

diff devsim_test1_gold.json $FILENAME_01_TEMP1 >> $FILENAME_01_STDOUT
[ $? -eq 0 ] || fail_msg "test1 diff comparison"

#############################################################################
# Test 2: Verify devsim results using vulkaninfo.

FILENAME_02_TEMP1="devsim_test2_temp1.json"
FILENAME_02_TEMP2="devsim_test2_temp2.json"
rm -f $FILENAME_02_TEMP1 $FILENAME_02_TEMP2

export VK_DEVSIM_FILENAME="devsim_test2_in1.json:devsim_test2_in2.json:devsim_test2_in3.json:devsim_test2_in4.json:devsim_test2_in5.json"
"$LVL_BUILD_DIR/demos/vulkaninfo" -j > $FILENAME_02_TEMP1 2> /dev/null
[ $? -eq 0 ] || fail_msg "test2 vulkaninfo"

# Use jq to extract, reformat, and sort the output.
JSON_SECTIONS='{VkPhysicalDeviceProperties,VkPhysicalDeviceFeatures,VkPhysicalDeviceMemoryProperties,ArrayOfVkQueueFamilyProperties,ArrayOfVkFormatProperties}'
jq -S $JSON_SECTIONS $FILENAME_02_TEMP1 > $FILENAME_02_TEMP2
[ $? -eq 0 ] || fail_msg "test2 jq extraction"

jq --slurp  --exit-status '.[0] == .[1]' devsim_test2_gold.json $FILENAME_02_TEMP2 > /dev/null
[ $? -eq 0 ] || fail_msg "test2 jq comparison"

#############################################################################

printf "$GREEN[  PASSED  ]$NC $0\n"
exit 0

# vim: set sw=4 ts=8 et ic ai:
