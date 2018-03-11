#!/bin/sh

echo `date`"  has restart";

#发送http请求

#curl "http://wechat.dami.sh/wechatinterface.aspx?server=passport_svr|115.159.36.200|36001&state=0&timestamp="`date +%s`

if [ $# == 1 ];then
	if [ -f $1 ];then
		time=`date +'%Y-%m-%d_%H:%M:%S'`
		mv $1 $1.$time
		echo  "mv $1 to $1.$time"
	else
		echo "no file : $1"
	fi
fi

echo ""
echo "------------------------------------------------------------"
