#!/bin/bash

if [ ! -f ./.cache/ok ]; then
    ./scripts/download.sh
fi

if [ ! -f ./build/pathtracer ]; then
    ./scripts/cmake.sh
    make -C ./build -j4
fi

./build/pathtracer $1 $2 $3

