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

export LD_LIBRARY_PATH=${PWD}/../loader:${LD_LIBRARY_PATH}
export VK_LAYER_PATH=${PWD}/../layersvt

function trace_replay {
	PGM=$1
	VKTRACE=${PWD}/../vktrace/vktrace
	VKREPLAY=${PWD}/../vktrace/vkreplay
	APPDIR=${PWD}/../demos
	printf "$GREEN[ TRACE    ]$NC ${PGM}\n"
	${VKTRACE}	--Program ${APPDIR}/${PGM} \
			--Arguments "--c 100" \
			--WorkingDir ${APPDIR} \
			--OutputTrace ${PGM}.vktrace \
			-s 1
	printf "$GREEN[ REPLAY   ]$NC ${PGM}\n"
	${VKREPLAY}	--TraceFile ${PGM}.vktrace \
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

trace_replay cube

exit 0

