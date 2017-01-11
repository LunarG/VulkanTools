#!/bin/bash
# Update source for glslang, spirv-tools

set -e

if [[ $(uname) == "Linux" ]]; then
    CURRENT_DIR="$(dirname "$(readlink -f ${BASH_SOURCE[0]})")"
    CORE_COUNT=$(nproc || echo 4)
elif [[ $(uname) == "Darwin" ]]; then
    CURRENT_DIR="$(dirname "$(python -c 'import os,sys;print(os.path.realpath(sys.argv[1]))' ${BASH_SOURCE[0]})")"
    CORE_COUNT=$(sysctl -n hw.ncpu || echo 4)
fi
echo CORE_COUNT=$CORE_COUNT

REVISION_DIR="$CURRENT_DIR/external_revisions"

GLSLANG_REVISION=$(cat "${REVISION_DIR}/glslang_revision")
SPIRV_TOOLS_REVISION=$(cat "${REVISION_DIR}/spirv-tools_revision")
SPIRV_HEADERS_REVISION=$(cat "${REVISION_DIR}/spirv-headers_revision")
JSONCPP_REVISION=$(cat "${REVISION_DIR}/jsoncpp_revision")
echo "GLSLANG_REVISION=${GLSLANG_REVISION}"
echo "SPIRV_TOOLS_REVISION=${SPIRV_TOOLS_REVISION}"
echo "SPIRV_HEADERS_REVISION=${SPIRV_HEADERS_REVISION}"
echo "JSONCPP_REVISION=${JSONCPP_REVISION}"

BUILDDIR=${CURRENT_DIR}
BASEDIR="$BUILDDIR/external"

function create_glslang () {
   rm -rf "${BASEDIR}"/glslang
   echo "Creating local glslang repository (${BASEDIR}/glslang)."
   mkdir -p "${BASEDIR}"/glslang
   cd "${BASEDIR}"/glslang
   git clone https://github.com/KhronosGroup/glslang.git .
   git checkout ${GLSLANG_REVISION}
}

function update_glslang () {
   echo "Updating ${BASEDIR}/glslang"
   cd "${BASEDIR}"/glslang
   git fetch --all
   git checkout --force ${GLSLANG_REVISION}
}

function create_spirv-tools () {
   rm -rf "${BASEDIR}"/spirv-tools
   echo "Creating local spirv-tools repository (${BASEDIR}/spirv-tools)."
   mkdir -p "${BASEDIR}"/spirv-tools
   cd "${BASEDIR}"/spirv-tools
   git clone https://github.com/KhronosGroup/SPIRV-Tools.git .
   git checkout ${SPIRV_TOOLS_REVISION}
   mkdir -p "${BASEDIR}"/spirv-tools/external/spirv-headers
   cd "${BASEDIR}"/spirv-tools/external/spirv-headers
   git clone https://github.com/KhronosGroup/SPIRV-Headers .
   git checkout ${SPIRV_HEADERS_REVISION}
}

function update_spirv-tools () {
   echo "Updating ${BASEDIR}/spirv-tools"
   cd "${BASEDIR}"/spirv-tools
   git fetch --all
   git checkout ${SPIRV_TOOLS_REVISION}
   if [ ! -d "${BASEDIR}/spirv-tools/external/spirv-headers" -o ! -d "${BASEDIR}/spirv-tools/external/spirv-headers/.git" ]; then
      mkdir -p "${BASEDIR}"/spirv-tools/external/spirv-headers
      cd "${BASEDIR}"/spirv-tools/external/spirv-headers
      git clone https://github.com/KhronosGroup/SPIRV-Headers .
   else
      cd "${BASEDIR}"/spirv-tools/external/spirv-headers
      git fetch --all
   fi
   git checkout ${SPIRV_HEADERS_REVISION}
}

function build_glslang () {
   echo "Building ${BASEDIR}/glslang"
   cd "${BASEDIR}"/glslang
   mkdir -p build
   cd build
   cmake -D CMAKE_BUILD_TYPE=Release ..
   make -j $CORE_COUNT
   make install
}

function build_spirv-tools () {
   echo "Building ${BASEDIR}/spirv-tools"
   cd "${BASEDIR}"/spirv-tools
   mkdir -p build
   cd build
   cmake -D CMAKE_BUILD_TYPE=Release ..
   make -j $CORE_COUNT
}

function create_jsoncpp () {
   rm -rf ${BASEDIR}/jsoncpp
   echo "Creating local jsoncpp repository (${BASEDIR}/jsoncpp)."
   mkdir -p ${BASEDIR}/jsoncpp
   cd ${BASEDIR}/jsoncpp
   git clone https://github.com/open-source-parsers/jsoncpp.git .
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

INCLUDE_GLSLANG=false
INCLUDE_SPIRV_TOOLS=false
INCLUDE_JSONCPP=false

if [ "$#" == 0 ]; then
  # If no options are provided, build everything
  echo "Building glslang, spirv-tools, and jsoncpp"
  INCLUDE_GLSLANG=true
  INCLUDE_SPIRV_TOOLS=true
  INCLUDE_JSONCPP=true
else
  # If any options are provided, just compile those tools
  while [[ $# > 0 ]]
  do
    option="$1"

    case $option in
        # options to specify build of glslang components
        -g|--glslang)
        INCLUDE_GLSLANG=true
        echo "Building glslang ($option)"
        ;;

        # options to specify build of spirv-tools components
        -s|--spirv-tools)
        INCLUDE_SPIRV_TOOLS=true
        echo "Building spirv-tools ($option)"
        ;;

        # options to specify build of jsoncpp components
        -j|--jsoncpp)
        INCLUDE_JSONCPP=true
        echo "Building jsoncpp ($option)"
        ;;

        *)
        echo "Unrecognized option: $option"
        echo "Try the following:"
        echo " -g | --glslang      # enable glslang"
        echo " -s | --spirv-tools  # enable spirv-tools"
        echo " -j | --jsoncpp      # enable jsoncpp"
        exit 1
        ;;
    esac
    shift
  done
fi

if [ ${INCLUDE_GLSLANG} == "true" ]; then
  if [ ! -d "${BASEDIR}/glslang" -o ! -d "${BASEDIR}/glslang/.git" -o -d "${BASEDIR}/glslang/.svn" ]; then
     create_glslang
  fi
  update_glslang
  build_glslang
fi

if [ ${INCLUDE_SPIRV_TOOLS} == "true" ]; then
    if [ ! -d "${BASEDIR}/spirv-tools" -o ! -d "${BASEDIR}/spirv-tools/.git" ]; then
       create_spirv-tools
    fi
    update_spirv-tools
    build_spirv-tools
fi

if [ ${INCLUDE_JSONCPP} == "true" ]; then
    if [ ! -d "${BASEDIR}/jsoncpp" -o ! -d "${BASEDIR}/jsoncpp/.git" ]; then
       create_jsoncpp
    fi
    update_jsoncpp
    build_jsoncpp
fi
