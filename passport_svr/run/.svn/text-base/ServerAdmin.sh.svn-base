cd `dirname $0`

PORT=36001
BIN_FILE="bin/passport_svr"
VALGRIND_LOG_FILE="netlogsvr.valgrind"

#cd /data/test_passport/run_$PORT

copyright(){
    echo ""
    echo "#################################################"
    echo "#              Serve Admin Tool 1.2.0           #"
    echo "#################################################"
    echo ""
    echo Listen:$PORT
    echo ""
}

copyright

if [ $1 == stop ];then
        echo "stop..."
        kill `cat ./bin/protect.pid`
        kill `cat ./$BIN_FILE.pid`
elif [ $1 == frocekill ];then
        echo "forcekill..."
        kill -9 `cat ./bin/protect.pid`
        kill -9 `cat ./$BIN_FILE.pid`
elif [ $1 == start ];then
        echo "start..."
        if [ $2 == -f ];then
		./bin/protect -s "/bin/sh OnRestart.sh" $BIN_FILE $PORT  
        elif [ $2 == -v ];then
		mv nohup.out nohup.bak.`date +'%Y-%m-%d_%H:%M:%S'`
		nohup bin/protect -s "/bin/sh OnRestart.sh $VALGRIND_LOG_FILE" /usr/bin/valgrind --tool=memcheck --leak-check=full --track-origins=yes --log-file=$VALGRIND_LOG_FILE $BIN_FILE $PORT  &
        else
		mv nohup.out nohup.bak.`date +'%Y-%m-%d_%H:%M:%S'`
		 nohup bin/protect -s "/bin/sh OnRestart.sh" $BIN_FILE $PORT  &
        fi
elif [ $1 == restart ];then
        kill -9 `cat ./bin/protect.pid`
        kill -9 `cat ./$BIN_FILE.pid`
        sleep 5
	mv nohup.out nohup.bak.`date +'%Y-%m-%d_%H:%M:%S'`
         nohup bin/protect -s "/bin/sh OnRestart.sh" $BIN_FILE $PORT  &
elif [ $1 == clear ];then
        echo "clear..."
        rm ./logs/*
fi

sleep 1
ps x|grep $PORT

