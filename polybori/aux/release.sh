#!/bin/sh

export TMPDIR="PB$$"
export CVSROOT=":ext:alexanderdreyer@polybori.cvs.sourceforge.net:/cvsroot/polybori"


export OLDDIR=`pwd`
mkdir $TMPDIR
cd $TMPDIR

cvs -d $CVSROOT checkout PolyBoRi

cd PolyBoRi

scons distribute $@

scons srpm $@

cd $OLDDIR

cp $TMPDIR/*tar.gz .

cp `find $TMPDIR -name \*.rpm` .

rm -rf $TMPDIR
