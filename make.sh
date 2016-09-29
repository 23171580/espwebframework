#!/bin/bash
function build_target()
{
	DIR=`pwd`
	TYPE=%Y年%m月%d日%H时%M分%S秒
	STARTING=`date +$TYPE`
	START_TIME=`date +%s`
	START=`date +$TYPE`编译日志.txt
	export PATH=$PATH:$DIR/esp-open-sdk/xtensa-lx106-elf/bin
	export ESP8266SDK=$DIR/esp-open-sdk/SDK
	make | tee $START
	DATE=`date +$TYPE`编译完成.txt 
	COMPLETEING=`date +$TYPE`
	END_TIME=`date +%s`
	let "ELAPSED_TIME=$END_TIME-$START_TIME"
	echo -e "<<<开始编译时间:$STARTING>>>\n总共花费了$ELAPSED_TIME秒\n<<<编译完成时间:$COMPLETEING>>>"
	echo -e "<<<开始编译时间:$STARTING>>>\n总共花费了$ELAPSED_TIME秒\n<<<编译完成时间:$COMPLETEING>>>" > $DATE
}
build_target
echo 编译结束 !!!
exit
