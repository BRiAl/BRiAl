#! /bin/sh
CREATE="ln -s"
if test -d include
  then
    :
  else
    mkdir include
    cd include
    $CREATE ../cudd/cudd.h .
    $CREATE ../cudd/cuddInt.h .
    $CREATE ../epd/epd.h .
    $CREATE ../dddmp/dddmp.h .
    $CREATE ../mtr/mtr.h .
    $CREATE ../obj/cuddObj.hh .
    $CREATE ../st/st.h .
    $CREATE ../util/util.h .
    $CREATE ../mnemosyne/mnemosyne.h .
fi

# renaming util to cuddutil
if test -d cuddutil
  then
    :
  else
    mkdir cuddutil
fi

cd cuddutil
$CREATE -f ../util/libutil.a libcuddutil.a
