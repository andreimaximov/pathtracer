#! /bin/bash

BASEDIR="$(cd "$(dirname "$0")" && pwd -P)"
BUILD="${BASEDIR}/build"

mkdir -p ${BUILD}
rm -rf ${BUILD}/*
cd ${BUILD}
cmake -DCMAKE_BUILD_TYPE=Release ..
cd ${BASEDIR}
