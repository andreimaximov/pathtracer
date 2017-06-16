#! /bin/bash

SCRIPTDIR=$(cd $(dirname ${0}) && pwd -P)
ENV="${SCRIPTDIR}/env.sh"

source ${SCRIPTDIR}/env.sh

EXE="${PADIR}/build/pathtracer"

if [ ! -f ${EXE} ]; then
    echo "Please compile the pathtracer in the build directory."
    exit 1
fi

DIR="${CACHE}/lost-empire"
ZIP="lost-empire.zip"
URL="http://graphics.cs.williams.edu/data/meshes/lost-empire.zip"
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

${EXE} "${DIR}/lost_empire.obj" ${DIR} "${CONFIG}/lost-empire.json"
