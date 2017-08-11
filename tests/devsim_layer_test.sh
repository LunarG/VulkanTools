#! /bin/bash
# simple test of devsim layer

set errexit
set nounset
set physical

cd $(dirname "${BASH_SOURCE[0]}")

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

export LD_LIBRARY_PATH=${PWD}/../loader:${LD_LIBRARY_PATH}
export VK_LAYER_PATH=${PWD}/../layersvt
export VK_INSTANCE_LAYERS="VK_LAYER_LUNARG_device_simulation"

# Test input datafile, and filename of output.
FILENAME_01_IN="devsim_test1.json"
FILENAME_01_GOLD="devsim_test1_gold.json"
FILENAME_01_OUT="device_simulation_layer_test_1.json"

export VK_DEVSIM_FILENAME="${PWD}/${FILENAME_01_IN}"

#export VK_DEVSIM_DEBUG_ENABLE="1"
#export VK_DEVSIM_EXIT_ON_ERROR="1"
#export VK_LOADER_DEBUG="all"

VKJSON_INFO="${PWD}/../libs/vkjson/vkjson_info"

${VKJSON_INFO} > /dev/null

# compare vkjson output against gold
NUM_LINES=$(cut -f1 -d' ' <(wc -l ${FILENAME_01_GOLD}))
diff ${FILENAME_01_GOLD} <(head -n ${NUM_LINES} ${FILENAME_01_OUT}) >/dev/null
RES=$?
rm ${FILENAME_01_OUT}

if [ "$RES" -eq 0 ] ; then
   printf "$GREEN[  PASSED  ]$NC ${PGM}\n"
else
   printf "$RED[  FAILED  ]$NC file compare failed\n"
   printf "TEST FAILED\n"
   exit 1
fi

exit 0

# vim: set sw=4 ts=8 et ic ai:
