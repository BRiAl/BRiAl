#!/bin/sh

export TMPDIR="PB$$"
export CVSROOT=":pserver:anonymous@polybori.cvs.sourceforge.net:/cvsroot/polybori"


export OLDDIR=`pwd`
mkdir $TMPDIR
cd $TMPDIR

cvs -d $CVSROOT checkout PolyBoRi

cd PolyBoRi

scons distribute $@

scons srpm $@

cd $OLDDIR

cp $TMPDIR/PolyBoRi/*tar.gz .

cp `find $TMPDIR/PolyBoRi -name \*.rpm` .

rm -rf $TMPDIR
