#!/bin/sh

SEDFILE=$HOME/.tmpbeauty$$.sed
echo '#!/bin/sed -f' > $SEDFILE


for data in $(sed -e "s/sec/sec\n/g" $@ | grep -o "[0-9.]* *sec"| sed -e "s/ /_/g")
do
DATASTR="`units -t -o %.3f ${data%_sec}s 1s`\\\\secunit/"

for DATASTART in "\([^.0-9]\)$data/\1" "^$data/"
do
  echo "s/$DATASTART$DATASTR" | sed "s/_/ /" >> $SEDFILE
done

done


for data in $(sed -e "s/KB/KB\n/g" $@ | grep -o "[0-9.]* *KB"| sed -e "s/ /_/")
do
echo "s/$data/`units -t -o %.2f ${data%_KB}KiBi 1MiBi` MB/g"| \
    sed "s/_/ /" >> $SEDFILE
done
echo 's/ *MB/\\MBunit/g' >> $SEDFILE

sed -f $SEDFILE $@

#cat $SEDFILE
rm $SEDFILE
