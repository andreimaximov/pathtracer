#! /bin/bash

BASEDIR=$(dirname "$0")

cpplint --filter=-legal/copyright,-build/include,-build/c++11,-runtime/references,-runtime/indentation_namespace \
        --repository=${BASEDIR} \
        --root=include \
        --exclude=${BASEDIR}/build \
        --extensions=hpp,cpp \
        --recursive \
        ${BASEDIR}/src ${BASEDIR}/include
