#! /bin/bash

SCRIPTDIR=$(cd $(dirname ${0}) && pwd -P)
ENV="${SCRIPTDIR}/env.sh"

source ${SCRIPTDIR}/env.sh

EXE="${PADIR}/build/pathtracer"

if [ ! -f ${EXE} ]; then
    echo "Please compile the pathtracer in the build directory."
    exit 1
fi

DIR="${CACHE}/rungholt"
ZIP="rungholt.zip"
URL="http://graphics.cs.williams.edu/data/meshes/rungholt.zip"
CWD=$(pwd)

if [[ ! -d ${DIR} ]]; then
    echo "Downloading scene..."
    mkdir -p ${DIR}
    cd ${DIR}
    curl -o ${ZIP} ${URL}
    unzip ${ZIP}
    rm ${ZIP}
    cd ${CWD}
fi

${EXE} "${DIR}/rungholt.obj" ${DIR} "${CONFIG}/rungholt.json"
