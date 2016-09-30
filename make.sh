#! /bin/bash
DIR=`pwd`
export SDKBASE=$DIR/
export PATH=$PATH:$DIR/ESP_OPEN_SDK/xtensa-lx106-elf/bin
make $*
