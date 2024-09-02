#!/usr/bin/env bash

set -exu

COMPILER="${CC:-clang++-18}"
EXECUTABLE=${1/.cpp/}

$COMPILER $1 -o $EXECUTABLE -std=c++20 -O0 -ggdb -fsanitize=address,undefined
./$EXECUTABLE
