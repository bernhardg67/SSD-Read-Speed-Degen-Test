#!/bin/sh
PRGNAME=degentest
cmake -S . -B build
make -C build
mv build/$PRGNAME .
strip -s $PRGNAME