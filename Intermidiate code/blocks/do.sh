#!/bin/sh

if [ "$1" != "" ]; then
    echo "Compiling $1"
    ./dana < $1 > a.s || exit 1
    as -o a.o a.s
    ld a.o libdana.a
fi
