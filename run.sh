#!/bin/bash

set -e

dir="$1\/day$(printf '%02d' $2)"
path="$dir\/part$3.c"

cat CMakeLists-Copy.txt | sed "s/\$\$PATH/$path/g" > CMakeLists.txt

if [ "$4" = "-d" ]; then
    sed -i "s/\$\$BUILDTYPE/Debug/g" CMakeLists.txt
else
    sed -i "s/\$\$BUILDTYPE/Release/g" CMakeLists.txt
fi

mkdir -p build
cd build
cmake ..
make

WD="$(pwd)"

clear
cd "../$1/day$(printf '%02d' $2)/"
shift 3

if [ "$1" = "-d" ] ; then
    shift 1
    valgrind "$@" "$WD/AOC"
else
    $WD/AOC "$@"
fi
