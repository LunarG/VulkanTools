#!/bin/sh

BUILDDIR=${PWD}
BASEDIR="${BUILDDIR}/submodules"

git submodule update --init --recursive

echo "Building ${BASEDIR}/jsoncpp"
cd ${BASEDIR}/jsoncpp
python amalgamate.py

# Remove the submodules git metadata in prep for creating an orig.tgz
find ${BASEDIR} -type d -name .git -exec rm -fr {} \; 
