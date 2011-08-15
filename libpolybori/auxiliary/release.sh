#!/bin/sh

VALUE=`echo "_$1" | grep "^_-"`

while [ "$VALUE" != "" ]; do
  if [ "$1" = "-h" -o "$1" = "--help" ]; then
    echo "usage: $0 [<opts>] <arg1> [<arg2> ...]"
    echo ""
    echo "Do the PolyBoRi release"
    echo "<opts> may be a sequence of the following options:"
    echo "  -f, --force: tag  even if tag is already existing"
    echo "  -n, --non-changing: do not make changes on repos"
    echo "  -t, --tag: use the tag <arg1> (default is read from versionnumber)"
    echo "  -h, --help:  print this usage text and exit"
    exit 0
  fi

  if [ "$1" = "-t" -o "$1" = "--tag" ]; then
    shift
    if [ "$1" = "-f" -o "$1" = "--force" ]; then
      FORCED="-f"
      shift
    fi
    HG_TAG="$1"
  fi

  if [ "$1" = "-f" -o "$1" = "--force" ]; then
    FORCED="-f"
  fi
  if [ "$1" = "-n" -o "$1" = "--non-changing" ]; then
    echo "Note: not changing repositories!"
    EXEC="echo Skipping: "
  fi
  shift
  VALUE=`echo "_$1" | grep "^_-"`
done

export TMPDIR="/tmp/$USER/PB$$"
export BUILDDIR="$TMPDIR/polybori"
export HG_SF="http://polybori.hg.sourceforge.net:8000/hgroot/polybori/polybori"

OLDPWD=${PWD}
THIS="$0"

# Follow all symbolic links to get the real file name and directory
while [ -L ${THIS} ]; do
    THIS_DIR=`dirname "${THIS}"`
    # emulating readlink, which is not available everywhere
    ls_l=`LC_ALL=C ls -l "${THIS}" 2> /dev/null`
    THIS="${ls_l/* -> }"
    cd "${THIS_DIR}"
done
THIS_DIR=`dirname "${THIS}"`
cd ${OLDPWD}

# go to main directory 
cd $THIS_DIR/../../

VERSION_NUMBER=`sed -e "s/-/./g" versionnumber`
RELEASE_DIR="$OLDPWD/release-$VERSION_NUMBER"
RELEASE_TAG="v${VERSION_NUMBER}"
if [ "${HG_TAG}" = "" ]; then
  echo "Using versionnnumber for tagging!"
  HG_TAG="$RELEASE_TAG"
fi

echo "Releasing PolyBoRi release ${HG_TAG}"

$EXEC hg tag $FORCE $HG_TAG || (echo "Release already tagged!"; exit 1)
$EXEC hg push || (echo "Pushing to devel repo failed"; exit 1)
$EXEC hg push $HG_SF  || (echo "Pushing to official Repo failed"; exit 1)

rm -rf $TMPDIR
mkdir $TMPDIR
cd $TMPDIR 

hg clone $HG_SF

cd $BUILDDIR

scons distribute USE_TIMESTAMP=no $@

scons prepare-rpm $@
scons srpm $@

TARBALL=`ls *tar.gz`
rm -rf $RELEASE_DIR
mkdir $RELEASE_DIR

patt1=`echo $RELEASE_TAG| sed -e "s/v\([^.]*\.[^.]*\)\..*/\1/" `
patt2=${HG_TAG/v/}

tarresult=${TARBALL/$patt1/$patt2}

cp $BUILDDIR/$TARBALL $RELEASE_DIR/$tarresult

cp `find pkgs -name "*.src.rpm" -o -name "*.spec"  -o -name "*.tar.*"` $RELEASE_DIR/ \
|| (echo "Copying results failed!" ; exit 1)

hg tags |grep "$RELEASE_TAG" 
rm -rf $TMPDIR
