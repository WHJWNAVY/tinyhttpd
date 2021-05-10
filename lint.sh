#!/bin/sh

SRC=./
PLAT=unix64

find ${SRC} -iname "*.[ch]" | xargs -r clang-format -i -style=file
find ${SRC} \( -iname "*.[ch]" -o -iname Makefile -o -iname CMakeLists.txt -o -iname README.md -o -iname config.in -o -iname "*.patch" -o -iname "*.pc.in" \) -executable | xargs -r chmod -x

cppcheck --enable=all --platform=${PLAT} -q ${SRC}

