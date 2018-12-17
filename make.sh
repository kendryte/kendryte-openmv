#!/bin/sh

cd ./build
cmake .. -DPROJ=$1 -DTOOLCHAIN=/home/xel/kendryte-toolchain-8.2/bin && make
