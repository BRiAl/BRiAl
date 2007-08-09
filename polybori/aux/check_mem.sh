#!/bin/sh

$@ &


PID=$!

#echo $PID 

VSZ=1
RSS=1
VSZnew=1
RSSnew=1

while [ "$VSZnew" ]
do
DATA=`ps -o vsz -o rss -p $PID`


if [ $VSZnew -gt $VSZ ]
    then VSZ=$VSZnew
fi
if [ $RSSnew -gt $RSS ]
    then RSS=$RSSnew
fi
sleep 0.1
#echo $VSZ $RSS


VSZnew=`echo $DATA | awk '{print $3}'`
RSSnew=`echo $DATA | awk '{print $4}'`
done


VSZ=`units -t -o %.2f ${VSZ}kibi 1mebi`
RSS=`units -t -o %.2f ${RSS}kibi 1mebi`
echo VSZ $VSZ MB RSS $RSS MB