#!/bin/bash
# Update source for glslang, LunarGLASS, spirv-tools

set -e

GLSLANG_REVISION=$(cat $PWD/glslang_revision)
SPIRV_TOOLS_REVISION=$(cat $PWD/spirv-tools_revision)
echo "GLSLANG_REVISION=$GLSLANG_REVISION"
echo "SPIRV_TOOLS_REVISION=$SPIRV_TOOLS_REVISION"

LUNARGLASS_REVISION=$(cat $PWD/LunarGLASS_revision)
echo "LUNARGLASS_REVISION=$LUNARGLASS_REVISION"

BUILDDIR=$PWD
BASEDIR=$BUILDDIR/external

function create_glslang () {
   rm -rf $BASEDIR/glslang
   echo "Creating local glslang repository ($BASEDIR/glslang)."
   mkdir -p $BASEDIR/glslang
   cd $BASEDIR/glslang
   git clone https://github.com/KhronosGroup/glslang.git .
   git checkout $GLSLANG_REVISION
}

function update_glslang () {
   echo "Updating $BASEDIR/glslang"
   cd $BASEDIR/glslang
   git fetch --all
   git checkout $GLSLANG_REVISION
   # Revert glslang a5c33d6ffb34ccede5b233bc724c907166b6e479
   # See https://github.com/KhronosGroup/Vulkan-LoaderAndValidationLayers/issues/681
   git diff-index --quiet HEAD | true
   rc=${PIPESTATUS[0]}
   if (( $rc == 0 ))
   then
      echo "applying patch to revert glslang a5c33d"
      git apply $BUILDDIR/glslang_revert_a5c33d.patch.txt
   fi
}

function create_spirv-tools () {
   rm -rf $BASEDIR/spirv-tools
   echo "Creating local spirv-tools repository ($BASEDIR/spirv-tools)."
   mkdir -p $BASEDIR/spirv-tools
   cd $BASEDIR/spirv-tools
   git clone https://github.com/KhronosGroup/SPIRV-Tools.git .
   git checkout $SPIRV_TOOLS_REVISION
   mkdir -p $BASEDIR/spirv-tools/external/spirv-headers
   cd $BASEDIR/spirv-tools/external/spirv-headers
   git clone https://github.com/KhronosGroup/SPIRV-Headers .
}

function update_spirv-tools () {
   echo "Updating $BASEDIR/spirv-tools"
   cd $BASEDIR/spirv-tools
   git fetch --all
   git checkout $SPIRV_TOOLS_REVISION
   if [ ! -d "$BASEDIR/spirv-tools/external/spirv-headers" -o ! -d "$BASEDIR/spirv-tools/external/spirv-headers/.git" ]; then
      mkdir -p $BASEDIR/spirv-tools/external/spirv-headers
      cd $BASEDIR/spirv-tools/external/spirv-headers
      git clone https://github.com/KhronosGroup/SPIRV-Headers .
   else
      cd $BASEDIR/spirv-tools/external/spirv-headers
      git fetch --all
      git pull
   fi
}

function build_glslang () {
   echo "Building $BASEDIR/glslang"
   cd $BASEDIR/glslang
   mkdir -p build
   cd build
   cmake -D CMAKE_BUILD_TYPE=Release ..
   make
   make install
}

function create_LunarGLASS () {
   rm -rf $BASEDIR/LunarGLASS
   echo "Creating local LunarGLASS repository ($BASEDIR/LunarGLASS)."
   mkdir -p $BASEDIR/LunarGLASS
   cd $BASEDIR/LunarGLASS
   git clone https://github.com/LunarG/LunarGLASS.git .
   mkdir -p Core/LLVM
   cd Core/LLVM 
   wget http://llvm.org/releases/3.4/llvm-3.4.src.tar.gz
   tar --gzip -xf llvm-3.4.src.tar.gz
   git checkout -f .  # put back the LunarGLASS versions of some LLVM files
   git checkout $LUNARGLASS_REVISION
   # copy overlay files
   cd $BASEDIR/LunarGLASS
   cp -R $BUILDDIR/LunarGLASS/* .
}

function update_LunarGLASS () {
   echo "Updating $BASEDIR/LunarGLASS"
   cd $BASEDIR/LunarGLASS
   git fetch
   git checkout -f .
   git checkout $LUNARGLASS_REVISION 
   # Figure out how to do this with git
   #git checkout $LUNARGLASS_REVISION |& tee gitout
   #if grep --quiet LLVM gitout ; then
   #   rm -rf $BASEDIR/LunarGLASS/Core/LLVM/llvm-3.4/build
   #fi
   #rm -rf gitout
 
   # copy overlay files
   cp -R $BUILDDIR/LunarGLASS/* .
}

function build_LunarGLASS () {
   echo "Building $BASEDIR/LunarGLASS"
   cd $BASEDIR/LunarGLASS/Core/LLVM/llvm-3.4
   if [ ! -d "$BASEDIR/LunarGLASS/Core/LLVM/llvm-3.4/build" ]; then
      mkdir -p build
      cd build
      ../configure --enable-terminfo=no --enable-curses=no
      REQUIRES_RTTI=1 make -j $(nproc) && make install DESTDIR=`pwd`/install
   fi
   cd $BASEDIR/LunarGLASS
   mkdir -p build
   cd build
   cmake -D CMAKE_BUILD_TYPE=Release ..
   make
   make install
}

function build_spirv-tools () {
   echo "Building $BASEDIR/spirv-tools"
   cd $BASEDIR/spirv-tools
   mkdir -p build
   cd build
   cmake -D CMAKE_BUILD_TYPE=Release ..
   make -j $(nproc)
}

# If any options are provided, just compile those tools
# If no options are provided, build everything
INCLUDE_GLSLANG=false
INCLUDE_SPIRV_TOOLS=false

if [ "$#" == 0 ]; then
  echo "Building glslang, spirv-tools"
  INCLUDE_GLSLANG=true
  INCLUDE_SPIRV_TOOLS=true
fi

# If any options are provided, just compile those tools
# If no options are provided, build everything
INCLUDE_LUNARGLASS=false

if [ "$#" == 0 ]; then
  echo "Building LunarGLASS"
  INCLUDE_LUNARGLASS=true
else
  # Parse options
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
        *)
        echo "Unrecognized option: $option"
        echo "Try the following:"
        echo " -g | --glslang      # enable glslang"
        echo " -s | --spirv-tools  # enable spirv-tools"
        echo " -l | --LunarGLASS   # enable LunarGLASS"
        exit 1
        ;;
    esac
    shift
  done
fi

if [ $INCLUDE_GLSLANG == "true" ]; then
  if [ ! -d "$BASEDIR/glslang" -o ! -d "$BASEDIR/glslang/.git" -o -d "$BASEDIR/glslang/.svn" ]; then
     create_glslang
  fi
  update_glslang
  build_glslang
fi


if [ $INCLUDE_SPIRV_TOOLS == "true" ]; then
    if [ ! -d "$BASEDIR/spirv-tools" -o ! -d "$BASEDIR/spirv-tools/.git" ]; then
       create_spirv-tools
    fi
    update_spirv-tools
    build_spirv-tools
fi
if [ $INCLUDE_LUNARGLASS == "true" ]; then
    if [ ! -d "$BASEDIR/LunarGLASS" -o ! -d "$BASEDIR/LunarGLASS/.git" ]; then
       create_LunarGLASS
    fi
    update_LunarGLASS
    build_LunarGLASS
fi

