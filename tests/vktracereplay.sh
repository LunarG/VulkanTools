#!/bin/bash
#set -x
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

LVL_BUILD=${PWD}/../submodules/Vulkan-ValidationLayers
export LD_LIBRARY_PATH=${LVL_BUILD}/loader:${LD_LIBRARY_PATH}
export VK_LAYER_PATH=${PWD}/../layersvt

function trace_replay {
	PGM=$1
	PARGS=$2
	TARGS=$3
	VKTRACE=${PWD}/../vktrace/vktrace
	VKREPLAY=${PWD}/../vktrace/vkreplay
	APPDIR=${LVL_BUILD}/demos
	printf "$GREEN[ TRACE    ]$NC ${PGM}\n"
	${VKTRACE}	--Program ${APPDIR}/${PGM} \
			--Arguments "--c 100 ${PARGS}" \
			--WorkingDir ${APPDIR} \
			--OutputTrace ${PGM}.vktrace \
			${TARGS} \
			-s 1
	printf "$GREEN[ REPLAY   ]$NC ${PGM}\n"
	${VKREPLAY}	--Open ${PGM}.vktrace \
			-s 1
	rm -f ${PGM}.vktrace
	cmp -s 1.ppm ${APPDIR}/1.ppm
	RES=$?
	rm 1.ppm ${APPDIR}/1.ppm
	if [ $RES -eq 0 ] ; then
	   printf "$GREEN[  PASSED  ]$NC ${PGM}\n"
	else
	   printf "$RED[  FAILED  ]$NC screenshot file compare failed\n"
	   printf "$RED[  FAILED  ]$NC ${PGM}\n"
	   printf "TEST FAILED\n"
	   exit 1
	fi
}

trace_replay cube "" "--PMB false"
# Test smoketest with pageguard
trace_replay smoketest "" "--PMB true"
# Test smoketest without pageguard, using push constants
trace_replay smoketest "-p" "--PMB false"
# Test smoketest without pageguard, using flush call
trace_replay smoketest "--flush" "--PMB false"

exit 0

