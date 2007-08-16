#!/bin/sh
PBMATCH="used clock[^0-9.]*\([0-9.]*\).*\nVSZ \([0-9.]*\).*"
SATMATCH="Memory used[^0-9.]*\([0-9.]*\).*\n[^0-9.]*\([0-9.]*\).*"

grep "CPU\|clock\|Memory\|VSZ\|File:" $@ | \
  sed -e ":a;\$!N" -e "s|% File: .*/\([^/]*\).py| \1 \&|" -e "s/$PBMATCH/ \1 sec \& \2 KB \&/;ta" \
    -e "s/$SATMATCH/ \2 sec \& \1 MB \\\\\\\\/;ta"\
    -e "P;D" 




