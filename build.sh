#!/bin/bash -e

git submodule update --init --recursive
west update

source ./env.sh
make CMAKE_BUILD_TYPE=Debug
make degu.bin
