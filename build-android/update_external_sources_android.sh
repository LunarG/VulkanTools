#!/bin/bash
# Update source for jsoncpp, Vulkan-Headers, Vulkan-Tools, and Vulkan-ValidationLayers

# Copyright 2016 The Android Open Source Project
# Copyright (C) 2015 Valve Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -e

ANDROIDBUILDDIR=$PWD
BUILDDIR=$ANDROIDBUILDDIR
BASEDIR=$BUILDDIR/third_party

JSONCPP_REVISION=$(cat $ANDROIDBUILDDIR/jsoncpp_revision_android)
VULKAN_TOOLS_REVISION=$(cat $ANDROIDBUILDDIR/vulkan-tools_revision_android)
VULKAN_HEADERS_REVISION=$(cat $ANDROIDBUILDDIR/vulkan-headers_revision_android)
VULKAN_VALIDATIONLAYERS_REVISION=$(cat $ANDROIDBUILDDIR/vulkan-validationlayers_revision_android)

echo "JSONCPP_REVISION=$JSONCPP_REVISION"
echo "VULKAN_TOOLS_REVISION=$VULKAN_TOOLS_REVISION"
echo "VULKAN_HEADERS_REVISION=$VULKAN_HEADERS_REVISION"
echo "VULKAN_VALIDATIONLAYERS_REVISION=$VULKAN_VALIDATIONLAYERS_REVISION"

JSONCPP_URL=$(cat $ANDROIDBUILDDIR/jsoncpp_url_android)
VULKAN_TOOLS_URL=$(cat $ANDROIDBUILDDIR/vulkan-tools_url_android)
VULKAN_HEADERS_URL=$(cat $ANDROIDBUILDDIR/vulkan-headers_url_android)
VULKAN_VALIDATIONLAYERS_URL=$(cat $ANDROIDBUILDDIR/vulkan-validationlayers_url_android)

echo "JSONCPP_URL=$JSONCPP_URL"
echo "VULKAN_TOOLS_URL=$VULKAN_TOOLS_URL"
echo "VULKAN_HEADERS_URL=$VULKAN_HEADERS_URL"

if [[ $(uname) == "Linux" ]]; then
    cores="$(nproc || echo 4)"
elif [[ $(uname) == "Darwin" ]]; then
    cores=$(sysctl -n hw.ncpu)
fi

#
# Parse parameters
#

function printUsage {
   echo "Supported parameters are:"
   echo "    --abi <abi> (optional)"
   echo "    --no-build (optional)"
   echo
   echo "i.e. ${0##*/} --abi arm64-v8a \\"
   exit 1
}

while [[ $# -gt 0 ]]
do
    case $1 in
        --abi)
            abi="$2"
            shift 2
            ;;
        --no-build)
            nobuild=1
            shift 1
            ;;
        *)
            # unknown option
            echo Unknown option: $1
            echo
            printUsage
            exit 1
            ;;
    esac
done

echo abi=$abi
if [[ -z $abi ]]
then
    echo No abi provided, so building for all supported abis.
fi

echo no-build=$nobuild
if [[ $nobuild ]]
then
    echo Skipping build.
fi

function create_jsoncpp () {
   rm -rf ${BASEDIR}/jsoncpp
   echo "Creating local jsoncpp repository (${BASEDIR}/jsoncpp)."
   mkdir -p ${BASEDIR}/jsoncpp
   cd ${BASEDIR}/jsoncpp
   git clone ${JSONCPP_URL} .
   git checkout ${JSONCPP_REVISION}
}

function update_jsoncpp () {
   echo "Updating ${BASEDIR}/jsoncpp"
   cd ${BASEDIR}/jsoncpp
   git fetch --all
   git checkout ${JSONCPP_REVISION}
}

function build_jsoncpp () {
   echo "Building ${BASEDIR}/jsoncpp"
   cd ${BASEDIR}/jsoncpp
   python amalgamate.py
}

function create_vulkan-headers () {
   rm -rf $BASEDIR/Vulkan-Headers
   echo "Creating local Vulkan-Headers repository ($BASEDIR/Vulkan-Headers)."
   mkdir -p $BASEDIR/Vulkan-Headers
   cd $BASEDIR/Vulkan-Headers
   git clone $VULKAN_HEADERS_URL .
   git checkout $VULKAN_HEADERS_REVISION
}

function update_vulkan-headers () {
   echo "Updating $BASEDIR/Vulkan-Headers"
   cd $BASEDIR/Vulkan-Headers
   if [[ $(git config --get remote.origin.url) != $VULKAN_HEADERS_URL ]]; then
      echo "Vulkan-Headers URL mismatch, recreating local repo"
      create_vulkan-headers
      return
   fi
   git fetch --all
   git checkout $VULKAN_HEADERS_REVISION
}
 
function create_vulkan-tools () {
   rm -rf $BASEDIR/Vulkan-Tools
   echo "Creating local Vulkan-Tools repository ($BASEDIR/Vulkan-Tools)."
   mkdir -p $BASEDIR/Vulkan-Tools
   cd $BASEDIR/Vulkan-Tools
   git clone $VULKAN_TOOLS_URL .
   git checkout $VULKAN_TOOLS_REVISION
   cd build-android
   ./update_external_sources_android.sh --no-build
}

function update_vulkan-tools () {
   echo "Updating $BASEDIR/Vulkan-Tools"
   cd $BASEDIR/Vulkan-Tools
   if [[ $(git config --get remote.origin.url) != $VULKAN_TOOLS_URL ]]; then
      echo "Vulkan-Tools URL mismatch, recreating local repo"
      create_vulkan-tools
      return
   fi
   git fetch --all
   git checkout $VULKAN_TOOLS_REVISION
   cd build-android
   ./update_external_sources_android.sh --no-build
}

function create_vulkan-validationlayers () {
   rm -rf $BASEDIR/Vulkan-ValidationLayers
   echo "Creating local Vulkan-ValidationLayers repository ($BASEDIR/Vulkan-ValidationLayers)."
   mkdir -p $BASEDIR/Vulkan-ValidationLayers
   cd $BASEDIR/Vulkan-ValidationLayers
   git clone $VULKAN_VALIDATIONLAYERS_URL .
   git checkout $VULKAN_VALIDATIONLAYERS_REVISION
   cd build-android
   ./update_external_sources_android.sh --no-build
}

function update_vulkan-validationlayers () {
   echo "Updating $BASEDIR/Vulkan-ValidationLayers"
   cd $BASEDIR/Vulkan-ValidationLayers
   if [[ $(git config --get remote.origin.url) != $VULKAN_VALIDATIONLAYERS_URL ]]; then
      echo "Vulkan-ValidationLayers URL mismatch, recreating local repo"
      create_vulkan-tools
      return
   fi
   git fetch --all
   git checkout $VULKAN_VALIDATIONLAYERS_REVISION
   cd build-android
   ./update_external_sources_android.sh --no-build
}

# Always init the submodules, which includes vulkan headers
echo "Initializing submodules"
git submodule update --init --recursive

if [ ! -d "$BASEDIR/Vulkan-Headers" -o ! -d "$BASEDIR/Vulkan-Headers/.git" ]; then
   create_vulkan-headers
fi
update_vulkan-headers

if [ ! -d "$BASEDIR/Vulkan-Tools" -o ! -d "$BASEDIR/Vulkan-Tools/.git" ]; then
   create_vulkan-tools
fi
update_vulkan-tools

if [ ! -d "$BASEDIR/Vulkan-ValidationLayers" -o ! -d "$BASEDIR/Vulkan-ValidationLayers/.git" ]; then
   create_vulkan-validationlayers
fi
update_vulkan-validationlayers

if [ ! -d "${BASEDIR}/jsoncpp" -o ! -d "${BASEDIR}/jsoncpp/.git" ]; then
   create_jsoncpp
fi
update_jsoncpp
build_jsoncpp

echo ""
echo "${0##*/} finished."

