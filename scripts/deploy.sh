#! /bin/bash

SCRIPTDIR=$(cd $(dirname ${0}) && pwd -P)
ENV="${SCRIPTDIR}/env.local.sh"

if [ ! -f ${ENV} ]; then
    echo "Please create ${ENV}!"
    exit 1
fi

source ${ENV}

BASE=`basename "${PADIR}"`
ARCHIVE="${WORKSPACE}/${BASE}.tar.gz"

echo "Creating archive..."
rm -rf ${ARCHIVE}
tar --exclude=.git --exclude=build --exclude=.cache -czf ${ARCHIVE} -C ${WORKSPACE} ${BASE}

echo "Uploading to host..."
scp -r ${ARCHIVE} "${USER}@${HOST}:${HOSTDIR}"

echo "Extracting..."
ssh "${USER}@${HOST}" "rm -rf ${HOSTDIR}/${BASE} &&
                       tar -xmf ${HOSTDIR}/${BASE}.tar.gz &&
                       rm ${HOSTDIR}/${BASE}.tar.gz"

# Cleanup
rm -rf ${ARCHIVE}
echo "Done!"
