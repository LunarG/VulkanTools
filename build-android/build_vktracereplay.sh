#!/bin/bash

set -ev

if [[ $(uname) == "Linux" ]]; then
    cores=$(ncpus)
elif [[ $(uname) == "Darwin" ]]; then
    cores=$(sysctl -n hw.ncpu)
fi

#
# build layers
#
./update_external_sources_android.sh
./android-generate.sh
ndk-build -j $cores

#
# create vkreplay APK
#
android update project -s -p . -t "android-23"
ant -buildfile vkreplay debug

#
# build cube-with-layers
#
(
pushd ../demos/android
android update project -s -p . -t "android-23"
ndk-build -j $cores
ant -buildfile cube-with-layers debug
popd
)

#
# build vktrace
#
(
pushd ..
./update_external_sources.sh -g -s
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_LOADER=Off -DBUILD_ICD=Off -DBUILD_TESTS=Off -DBUILD_LAYERS=Off -DBUILD_VKTRACEVIEWER=Off -DBUILD_LAYERSVT=Off -DBUILD_DEMOS=Off -DBUILD_VKJSON=Off -DBUILD_VKTRACE_LAYER=Off -DBUILD_VKTRACE_REPLAY=Off -DBUILD_VKTRACE=On ..
make -j $cores vktrace
popd
)

#
# build vktrace32
#
(
pushd ..
mkdir -p build32
cd build32
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_LOADER=Off -DBUILD_ICD=Off -DBUILD_TESTS=Off -DBUILD_LAYERS=Off -DBUILD_VKTRACEVIEWER=Off -DBUILD_LAYERSVT=Off -DBUILD_DEMOS=Off -DBUILD_VKJSON=Off -DBUILD_VKTRACE_LAYER=Off -DBUILD_VKTRACE_REPLAY=Off -DBUILD_X64=Off -DBUILD_VKTRACE=On ..
make -j $cores vktrace
popd
)

exit 0
