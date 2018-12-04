#!/bin/sh

if [ "$1" != "" ]; then
    echo "Compiling $1"
    ./minibasic < $1 > a.s || exit 1
    as -o a.o a.s
    ld a.o libminibasic.a
fi
