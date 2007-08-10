#!/bin/sh

MAXSECONDS=$1

shift
$@ &


PID=$!

#echo $PID 

PROCESS=1

PROCESSnew=1

START=`date +%s`
DIFF=0
notdone=1
while [ $notdone ]
do
DATA=`ps -o time $PID`
PROCESSnew=`echo $DATA |awk '{print $2}'`

sleep 1



STARTnew=`date +%s`

DIFF=$(($STARTnew - $START))

notdone=$PROCESSnew

if [ $DIFF -ge $MAXSECONDS ]
    then echo "% TIMEOUT after $DIFF seconds! Process time: $PROCESSnew" ; notdone=""
fi

done
