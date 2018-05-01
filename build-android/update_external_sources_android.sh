#!/bin/bash
# Update source for glslang, spirv-tools, shaderc

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

GLSLANG_REVISION=$(cat $ANDROIDBUILDDIR/glslang_revision_android)
SPIRV_TOOLS_REVISION=$(cat $ANDROIDBUILDDIR/spirv-tools_revision_android)
SPIRV_HEADERS_REVISION=$(cat $ANDROIDBUILDDIR/spirv-headers_revision_android)
SHADERC_REVISION=$(cat $ANDROIDBUILDDIR/shaderc_revision_android)
JSONCPP_REVISION=$(cat $ANDROIDBUILDDIR/jsoncpp_revision_android)

echo "GLSLANG_REVISION=$GLSLANG_REVISION"
echo "SPIRV_TOOLS_REVISION=$SPIRV_TOOLS_REVISION"
echo "SPIRV_HEADERS_REVISION=$SPIRV_HEADERS_REVISION"
echo "SHADERC_REVISION=$SHADERC_REVISION"
echo "JSONCPP_REVISION=$JSONCPP_REVISION"

GLSLANG_URL=$(cat $ANDROIDBUILDDIR/glslang_url_android)
SPIRV_TOOLS_URL=$(cat $ANDROIDBUILDDIR/spirv-tools_url_android)
SPIRV_HEADERS_URL=$(cat $ANDROIDBUILDDIR/spirv-headers_url_android)
SHADERC_URL=$(cat $ANDROIDBUILDDIR/shaderc_url_android)
JSONCPP_URL=$(cat $ANDROIDBUILDDIR/jsoncpp_url_android)

echo "GLSLANG_URL=$GLSLANG_URL"
echo "SPIRV_TOOL_URLS_=$SPIRV_TOOLS_URL"
echo "SPIRV_HEADERS_URL=$SPIRV_HEADERS_URL"
echo "SHADERC_URL=$SHADERC_URL"
echo "JSONCPP_URL=$JSONCPP_URL"

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

function create_glslang () {
   rm -rf $BASEDIR/shaderc/third_party/glslang
   echo "Creating local glslang repository ($BASEDIR/glslang)."
   mkdir -p $BASEDIR/shaderc/third_party/glslang
   cd $BASEDIR/shaderc/third_party/glslang
   git clone $GLSLANG_URL .
   git checkout $GLSLANG_REVISION
}

function update_glslang () {
   echo "Updating $BASEDIR/shaderc/third_party/glslang"
   cd $BASEDIR/shaderc/third_party/glslang
   if [[ $(git config --get remote.origin.url) != $GLSLANG_URL ]]; then
      echo "glslang URL mismatch, recreating local repo"
      create_glslang
      return
   fi
   git fetch --all
   git checkout $GLSLANG_REVISION
}

function create_spirv-tools () {
   rm -rf $BASEDIR/shaderc/third_party/spirv-tools
   echo "Creating local spirv-tools repository ($BASEDIR/shaderc/third_party/spirv-tools)."
   mkdir -p $BASEDIR/shaderc/third_party/spirv-tools
   cd $BASEDIR/shaderc/third_party/spirv-tools
   git clone $SPIRV_TOOLS_URL .
   git checkout $SPIRV_TOOLS_REVISION
}

function update_spirv-tools () {
   echo "Updating $BASEDIR/shaderc/third_party/spirv-tools"
   cd $BASEDIR/shaderc/third_party/spirv-tools
   if [[ $(git config --get remote.origin.url) != $SPIRV_TOOLS_URL ]]; then
      echo "spirv-tools URL mismatch, recreating local repo"
      create_spirv-tools
      return
   fi
   git fetch --all
   git checkout $SPIRV_TOOLS_REVISION
}

function create_spirv-headers () {
   rm -rf $BASEDIR/shaderc/third_party/spirv-tools/external/spirv-headers
   echo "Creating local spirv-headers repository ($BASEDIR/shaderc/third_party/spirv-tools/external/spirv-headers)."
   mkdir -p $BASEDIR/shaderc/third_party/spirv-tools/external/spirv-headers
   cd $BASEDIR/shaderc/third_party/spirv-tools/external/spirv-headers
   git clone $SPIRV_HEADERS_URL .
   git checkout $SPIRV_HEADERS_REVISION
}

function update_spirv-headers () {
   echo "Updating $BASEDIR/shaderc/third_party/spirv-tools/external/spirv-headers"
   cd $BASEDIR/shaderc/third_party/spirv-tools/external/spirv-headers
   if [[ $(git config --get remote.origin.url) != $SPIRV_HEADERS_URL ]]; then
      echo "spirv-headers URL mismatch, recreating local repo"
      create_spirv-headers
      return
   fi
   git fetch --all
   git checkout $SPIRV_HEADERS_REVISION
}

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

function create_shaderc () {
   rm -rf $BASEDIR/shaderc
   echo "Creating local shaderc repository ($BASEDIR/shaderc)."
   mkdir -p $BASEDIR/shaderc
   cd $BASEDIR/shaderc
   git clone $SHADERC_URL .
   git checkout $SHADERC_REVISION
}

function update_shaderc () {
   echo "Updating $BASEDIR/shaderc"
   cd $BASEDIR/shaderc
   if [[ $(git config --get remote.origin.url) != $SHADERC_URL ]]; then
      echo "shaderc URL mismatch, recreating local repo"
      create_shaderc
      return
   fi
   git fetch --all
   git checkout $SHADERC_REVISION
}

function build_shaderc () {
   echo "Building $BASEDIR/shaderc"
   cd $BASEDIR/shaderc/android_test
   if [[ $abi ]]; then
      ndk-build NDK_APPLICATION_MK=../../../jni/shaderc/Application.mk THIRD_PARTY_PATH=../third_party APP_ABI=$abi -j $cores;
   else
      ndk-build NDK_APPLICATION_MK=../../../jni/shaderc/Application.mk THIRD_PARTY_PATH=../third_party -j $cores;
   fi
}

if [ ! -d "$BASEDIR/shaderc" -o ! -d "$BASEDIR/shaderc/.git" ]; then
     create_shaderc
fi
update_shaderc

if [ ! -d "$BASEDIR/shaderc/third_party/glslang" -o ! -d "$BASEDIR/shaderc/third_party/glslang/.git" -o -d "$BASEDIR/shaderc/third_party/glslang/.svn" ]; then
   create_glslang
fi
update_glslang

if [ ! -d "$BASEDIR/shaderc/third_party/spirv-tools" -o ! -d "$BASEDIR/shaderc/third_party/spirv-tools/.git" ]; then
   create_spirv-tools
fi
update_spirv-tools

if [ ! -d "$BASEDIR/shaderc/third_party/spirv-tools/external/spirv-headers" -o ! -d "$BASEDIR/shaderc/third_party/spirv-tools/external/spirv-headers/.git" ]; then
   create_spirv-headers
fi
update_spirv-headers

if [[ -z $nobuild ]]
then
build_shaderc
fi

if [ ! -d "${BASEDIR}/jsoncpp" -o ! -d "${BASEDIR}/jsoncpp/.git" ]; then
   create_jsoncpp
fi
update_jsoncpp
build_jsoncpp

echo ""
echo "${0##*/} finished."

