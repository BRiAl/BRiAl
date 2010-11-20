#!/bin/sh

SEDFILE=$HOME/.tmpbeauty$$.sed
echo '#!/bin/sed -f' > $SEDFILE


for data in $(sed -e "s/sec/sec\n/g" $@ | grep -o "[0-9.][0-9.]* *sec"| sed -e "s/ /_/g")
do
datanum=`echo $data|sed -e "s/_*sec//"`


DATASTR="`units -t -o %.2f ${datanum}s 1s`\\\\secunit/"

for DATASTART in "\([^.0-9]\)$data/\1" "^$data/"
do
  echo "s/$DATASTART$DATASTR" | sed "s/_/ /g" >> $SEDFILE
done

done


for data in $(sed -e "s/KB/KB\n/g" $@ | grep -o "[0-9.]* *KB"| sed -e "s/ /_/")
do
datanum=`echo $data|sed -e "s/_*KB//"`
echo "s/$data/`units -t -o %.2f ${datanum}KiBi 1MiBi` MB/g"| \
    sed "s/_/ /" >> $SEDFILE
done


for data in $(sed -e "s/bytes/bytes\n/g" $@ | grep -o "[0-9.][0-9.]* *bytes"| sed -e "s/ /_/")
do

datanum=`echo $data|sed -e "s/_*bytes//"`

echo "s/$data/`units -t -o %.2f ${datanum}Bi 1MiBi` MB/g"| \
    sed "s/_/ /" >> $SEDFILE
done


OLDPWD=`pwd`



for data in $(grep -o "data.*.py" $@)
do

cd `dirname $0`/../../testsuite/py

repl=`python num_eq_vars.py $data|  sed -e "N" -e"s/Equations\(.*\)\nVariables\(.*\)/\2 NEXTTAB \1  NEXTTAB/" `


cd $OLDPWD
echo "s|.*$data.*|\\\\\\\\\n`basename $data |sed -e 's/.py//'`\& $repl|"  \
 >> $SEDFILE
 
done

echo 's/ *MB/\\MBunit/g' >> $SEDFILE


echo 's/.*VSZ.*//g' >> $SEDFILE
echo 's/NEXTTAB/\&/g' >> $SEDFILE
echo 's/Timed out.*/\\timedout/g' >> $SEDFILE
sed -f $SEDFILE $@



#cat $SEDFILE
rm $SEDFILE
