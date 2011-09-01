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
    echo "  -s, --syn: do not make changes on repos but syncing"
    echo "  -t, --tag: use the tag <arg1> (default is read from versionnumber)"
    echo "  -a, --auto:  use content of file 'versionnumber' for tagging"
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

  if [ "$1" = "-a" -o "$1" = "--auto" ]; then
    AUTO="True"
  fi

  if [ "$1" = "-f" -o "$1" = "--force" ]; then
    FORCED="-f"
  fi
  if [ "$1" = "-n" -o "$1" = "--non-changing" ]; then
    echo "Note: not changing repositories!"
    EXEC="echo Skipping: "
    EXECSYNC="echo Skipping: "
  fi
  if [ "$1" = "-s" -o "$1" = "--sync" ]; then
    echo "Note: not tagging!"
    EXEC="echo Skipping: "
    EXECSYNC=""
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
RELEASE_TAG="v${VERSION_NUMBER}"

if [ "${HG_TAG}" = "" ]; then
  if [ "${AUTO}" = "" ]; then
    echo "No tag given. Use -a|--auto for using versionnumber" 
    exit 1
  fi
    echo "Using versionnnumber for tagging!"
    HG_TAG="$RELEASE_TAG"
fi

RELEASE_DIR="$OLDPWD/release-${HG_TAG/v/}"

echo "Releasing PolyBoRi release ${HG_TAG}"

$EXEC hg tag $FORCED $HG_TAG || (echo "Release already tagged!"; exit 1)
$EXEC hg push || (echo "Pushing to devel repo failed"; exit 1)
# please set ssh site data in .ssh/config
$EXECSYNC hg push ssh://polybori/hgroot/polybori/polybori/ \
  || (echo "Pushing to official Repo failed"; exit 1)

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

hg parents
hg tags |grep "$RELEASE_TAG" 

mkdir $TMPDIR/debian
cd $TMPDIR/debian

patt3=${RELEASE_TAG/v/}
tardebian=${TARBALL/-$patt1/_$patt3.orig}

cp $BUILDDIR/$TARBALL $tardebian

tar -xvzf $tardebian
cd polybori-*

scons prepare-debian
rm -rf .scon* *.pyc

cd -
dpkg-source -b polybori-*  $tardebian || exit 1


rm -rf $TMPDIR
