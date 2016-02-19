#!/bin/bash
# Update source for LunarGLASS

set -e

LUNARGLASS_REVISION=$(cat $PWD/LunarGLASS_revision)
echo "LUNARGLASS_REVISION=$LUNARGLASS_REVISION"

BUILDDIR=$PWD
BASEDIR=$BUILDDIR/..

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
   cmake -D CMAKE_BUILD_TYPE=Release ..
   make
   make install
}

# Verify glslang is built
if [ ! -d "$BASEDIR/glslang" ]; then
  echo "glslang missing"
  if [ ! -d "$BASEDIR/LoaderAndValidationLayers" ]; then
    echo "LoaderAndValidationLayers  missing"
    echo "Install LoaderAndValidationLayers in $BASEDIR"
  fi
  echo "Run $BASEDIR/LoaderAndValidationLayers/update_external_sources.sh"
  exit 1
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
        # options to specify build of LunarGLASS components
        -l|--LunarGLASS)
        INCLUDE_LUNARGLASS=true
        echo "Building LunarGLASS ($option)"
        ;;
        *)
        echo "Unrecognized option: $option"
        echo "Try the following:"
        echo " -l | --LunarGLASS   # enable LunarGLASS"
        exit 1
        ;;
    esac
    shift
  done
fi

if [ $INCLUDE_LUNARGLASS == "true" ]; then
    if [ ! -d "$BASEDIR/LunarGLASS" -o ! -d "$BASEDIR/LunarGLASS/.git" ]; then
       create_LunarGLASS
    fi
    update_LunarGLASS
    build_LunarGLASS
fi

