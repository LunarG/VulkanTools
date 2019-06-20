#!/bin/bash

# Bash shell script to fix an apidump json file so that it ends with a full frame and is
# a legit json file. This script may be needed when a program outputs apidump in json
# format and is aborted or suspended, such that the last apidump frame is incomplete.
#
# Usage: FixApidumpJson.sh <inputfile>
#
# The fixed json is output to stdout.


# Copyright (c) 2019 The Khronos Group Inc.
# Copyright (c) 2019 Valve Corporation
# Copyright (c) 2019 LunarG, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Author: David Pinedo <david@lunarg.com>



# Validate parameter
if [ $# -ne 1 ] ; then
    echo "Usage: $0 <inputfile>" >&2
    exit 1
fi

if [ ! -r "$1" ] ; then
    echo "Cannot open file $1" >&2
    exit 1
fi

# Create a tmp file with the new ending for the output
export TFILE=/tmp/$$
echo -e ']\n}' >$TFILE

# Delete everyting in the input file from the start of the last frame to the end, and then append the new ending
tac $1 | sed '1,/.*"frameNumber" : ".*/d' | tail -n +3 | cat $TFILE  - |  tac

# Delete tmp file
rm $TFILE
