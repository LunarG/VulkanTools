#!/bin/bash

set -ev

#
# build layers
#
./update_external_sources_android.sh
./android-generate.sh
ndk-build -j $(ncpus)

#
# create vkreplay APK
#
android update project -s -p . -t "android-23"
ndk-build -j $(ncpus)
ant -buildfile vkreplay debug

#
# build cube-with-layers
#
(
pushd ../demos/android
android update project -s -p . -t "android-23"
ndk-build -j $(ncpus)
ant -buildfile cube-with-layers debug
popd
)

#
# build vktrace
#
(
pushd ..
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_VKTRACEVIEWER=Off ..
make -j $(ncpus)
popd
)

#
# build vktrace32
#
(
pushd ..
mkdir -p build32
cd build32
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_LOADER=Off -DBUILD_ICD=Off -DBUILD_TESTS=Off -DBUILD_LAYERS=Off -DBUILD_VKTRACEVIEWER=Off -DBUILD_LAYERSVT=Off -DBUILD_DEMOS=Off -DBUILD_VKJSON=Off -DBUILD_VKTRACE_LAYER=Off -DBUILD_VKTRACE_REPLAY=Off -DBUILD_X64=Off ..
make -j $(ncpus)
popd
)

exit 0
