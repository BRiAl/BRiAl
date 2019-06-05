#!/usr/bin/env bash

set -e

aclocal -I m4
libtoolize
autoreconf --install
automake --add-missing

