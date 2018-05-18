#!/bin/bash

# apidump_test.sh
# This script will run the demo vulkaninfo with the api_dump layer and capture the output.
# The script will search the output for a certain APIs that should appear in the output.
# If the threshold is met, the test will indicate PASS, else FAILURE.

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
cd ../submodules/Vulkan-ValidationLayers/demos

VK_ICD_FILENAMES=../icd/VkICD_mock_icd.json VK_LAYER_PATH=../../../layersvt VK_INSTANCE_LAYERS=VK_LAYER_LUNARG_api_dump ./vulkaninfo > apidump_file.tmp

printf "$GREEN[ RUN      ]$NC $0\n"
if [ -f apidump_file.tmp ]
then
    GPDFP_count=$(grep vkGetPhysicalDeviceFormatProperties apidump_file.tmp | wc -l)
    pipelineCacheUUID_count=$(grep pipelineCacheUUID apidump_file.tmp | wc -l)
    vk_format_feature_count=$(grep VK_FORMAT_FEATURE apidump_file.tmp | wc -l)
    if (( $GPDFP_count > 50 && $pipelineCacheUUID_count > 10 && $vk_format_feature_count > 1000 ))
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
