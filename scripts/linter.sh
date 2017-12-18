#!/bin/bash

cpplint --filter=-legal/copyright,-build/include,-build/c++11,-runtime/references,-runtime/indentation_namespace \
        --repository=. \
        --root=include \
        --exclude=./build \
        --extensions=hpp,cpp \
        --recursive \
        ./src ./include
