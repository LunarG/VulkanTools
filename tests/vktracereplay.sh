# Note: For this script to run successfully, cube must be in PATH.

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

export VK_LAYER_PATH=${PWD}/../layersvt

function trace_replay {
	PGM=`which $1`
	PARGS=$2
	TARGS=$3
	VKTRACE=${PWD}/../vktrace/vktrace
	VKREPLAY=${PWD}/../vktrace/vkreplay
	printf "$GREEN[ TRACE    ]$NC ${PGM}\n"
	${VKTRACE}	--Program ${PGM} \
			--Arguments "--c 100 ${PARGS}" \
			--WorkingDir ${PWD} \
			--OutputTrace ${PGM}.vktrace \
			${TARGS} \
			-s 1
	mv 1.ppm 1-trace.ppm
	printf "$GREEN[ REPLAY   ]$NC ${PGM}\n"
	${VKREPLAY}	--Open ${PGM}.vktrace \
			-s 1
	rm -f ${PGM}.vktrace
	cmp -s 1.ppm 1-trace.ppm
	RES=$?
	rm 1.ppm 1-trace.ppm
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

exit 0

