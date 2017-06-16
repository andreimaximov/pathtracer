#!/bin/sh

find         \
    src      \
    include  \
    tests    \
    -iname '*.cpp' -o -iname '*.hpp' | xargs clang-format -i -style=file

