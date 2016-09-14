#!/bin/bash
# Update source for glslang, LunarGLASS, spirv-tools

set -e

GLSLANG_REVISION=$(cat "${PWD}"/glslang_revision)
SPIRV_TOOLS_REVISION=$(cat "${PWD}"/spirv-tools_revision)
SPIRV_HEADERS_REVISION=$(cat "${PWD}"/spirv-headers_revision)
JSONCPP_REVISION=$(cat "${PWD}"/jsoncpp_revision)
echo "GLSLANG_REVISION=${GLSLANG_REVISION}"
echo "SPIRV_TOOLS_REVISION=${SPIRV_TOOLS_REVISION}"
echo "SPIRV_HEADERS_REVISION=${SPIRV_HEADERS_REVISION}"
echo "JSONCPP_REVISION=${JSONCPP_REVISION}"

LUNARGLASS_REVISION=$(cat "${PWD}"/LunarGLASS_revision)
echo "LUNARGLASS_REVISION=$LUNARGLASS_REVISION"

BUILDDIR=$PWD
BASEDIR=$BUILDDIR/external

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
   make -j $(nproc)
   make install
}

function create_LunarGLASS () {
   rm -rf "${BASEDIR}"/LunarGLASS
   echo "Creating local LunarGLASS repository (${BASEDIR}/LunarGLASS)."
   mkdir -p "${BASEDIR}"/LunarGLASS
   cd "${BASEDIR}"/LunarGLASS
   git clone https://github.com/LunarG/LunarGLASS.git .
   mkdir -p Core/LLVM
   cd Core/LLVM 
   wget http://llvm.org/releases/3.4/llvm-3.4.src.tar.gz
   tar --gzip -xf llvm-3.4.src.tar.gz
   git checkout -f $LUNARGLASS_REVISION . # put back the LunarGLASS versions of some LLVM files
   # copy overlay files
   cd "${BASEDIR}"/LunarGLASS
   cp -R "${BUILDDIR}"/LunarGLASS/* .
}

function update_LunarGLASS () {
   echo "Updating ${BASEDIR}/LunarGLASS"
   cd "${BASEDIR}"/LunarGLASS
   git fetch
   git checkout -f $LUNARGLASS_REVISION .
   # Figure out how to do this with git
   #git checkout $LUNARGLASS_REVISION |& tee gitout
   #if grep --quiet LLVM gitout ; then
   #   rm -rf $BASEDIR/LunarGLASS/Core/LLVM/llvm-3.4/build
   #fi
   #rm -rf gitout
 
   # copy overlay files
   cp -R "${BUILDDIR}"/LunarGLASS/* .
}

function build_LunarGLASS () {
   echo "Building ${BASEDIR}/LunarGLASS"
   cd "${BASEDIR}"/LunarGLASS/Core/LLVM/llvm-3.4
   if [ ! -d "${BASEDIR}/LunarGLASS/Core/LLVM/llvm-3.4/build" ]; then
      mkdir -p build
      cd build
      ../configure --enable-terminfo=no --enable-curses=no
      REQUIRES_RTTI=1 make -j $(nproc) && make install DESTDIR=`pwd`/install
   fi
   cd "${BASEDIR}"/LunarGLASS
   mkdir -p build
   cd build
   cmake -D CMAKE_BUILD_TYPE=Release ..
   make
   make install
}

function build_spirv-tools () {
   echo "Building ${BASEDIR}/spirv-tools"
   cd "${BASEDIR}"/spirv-tools
   mkdir -p build
   cd build
   cmake -D CMAKE_BUILD_TYPE=Release ..
   make -j $(nproc)
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
INCLUDE_LUNARGLASS=false
INCLUDE_JSONCPP=false

if [ "$#" == 0 ]; then
  # If no options are provided, build everything
  echo "Building glslang, spirv-tools, LunarGLASS, and jsoncpp"
  INCLUDE_GLSLANG=true
  INCLUDE_SPIRV_TOOLS=true
  INCLUDE_LUNARGLASS=true
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

        # options to specify build of LunarGLASS components
        -l|--LunarGLASS)
        INCLUDE_LUNARGLASS=true
        echo "Building LunarGLASS ($option)"
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
        echo " -l | --LunarGLASS   # enable LunarGLASS"
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

if [ $INCLUDE_LUNARGLASS == "true" ]; then
    if [ ! -d "${BASEDIR}/LunarGLASS" -o ! -d "${BASEDIR}/LunarGLASS/.git" ]; then
       create_LunarGLASS
    fi
    update_LunarGLASS
    build_LunarGLASS
fi

if [ ${INCLUDE_JSONCPP} == "true" ]; then
    if [ ! -d "${BASEDIR}/jsoncpp" -o ! -d "${BASEDIR}/jsoncpp/.git" ]; then
       create_jsoncpp
    fi
    update_jsoncpp
    build_jsoncpp
fi
