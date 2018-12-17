#!/bin/sh

K210_BIN_PATH=`pwd`/build

#here should set to yours
BRUN_SCRIPT_PATH=/home/xel/K210/pyScript

bin_file=$K210_BIN_PATH"/"$1".bin"

echo $bin_file

cd $BRUN_SCRIPT_PATH

if [ ! -n "$2" ] ;then
	python3 isp.py -b 115200 $bin_file -d /dev/ttyUSB0
else
	python3 isp.py -b 115200 $bin_file -d $2
fi

