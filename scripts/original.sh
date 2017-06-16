#! /bin/bash

SCRIPTDIR=$(cd $(dirname ${0}) && pwd -P)
ENV="${SCRIPTDIR}/env.sh"

source ${SCRIPTDIR}/env.sh

EXE="${PADIR}/build/pathtracer"

if [ ! -f ${EXE} ]; then
    echo "Please compile the pathtracer in the build directory."
    exit 1
fi

${EXE} "${SCENES}/CornellBox-Original.obj" ${SCENES} "${CONFIG}/cornell-box-original.json"
