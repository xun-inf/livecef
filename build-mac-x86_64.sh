#!/bin/bash

BUILD_PROJECT=0

BUILD_DIR="$(pwd)/.build/macos.x86_64"

while getopts bi flag
do
    case "${flag}" in
        b) BUILD_PROJECT=1;;
    esac
done

# Qt toolchain path: use QTDIR environment variable if set, otherwise
# auto-detect the first Qt kit under ~/Qt (Qt6 "macos" or Qt5 "clang_64")
if [ -z "${QTDIR}" ]; then
    QTDIR=$(ls -d "${HOME}"/Qt/*/macos "${HOME}"/Qt/*/clang_64 2>/dev/null | head -n 1)
fi

if [ -z "${QTDIR}" ]; then
    echo "ERROR: Qt SDK not found, please set the QTDIR environment variable"
    exit 1
fi

echo "Using Qt SDK: ${QTDIR}"

echo ============== Config project ==============
cmake -G "Xcode" -S . -B "${BUILD_DIR}" -DPROJECT_ARCH=x86_64 -DBUILD_DEMO=ON -DUSE_SANDBOX=ON -DQT_SDK_DIR:PATH="${QTDIR}" -DCMAKE_INSTALL_PREFIX:PATH="$(pwd)/out/macos.x86_64" $*

if [ ${BUILD_PROJECT} -eq 1 ]
then
    echo ============== Build project ==============
    cmake --build "${BUILD_DIR}"
fi