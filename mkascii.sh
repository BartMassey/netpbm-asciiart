#!/bin/sh
# Copyright © 2010 Bart Massey
# ALL RIGHTS RESERVED
# [This program is licensed under the "MIT License"]
# Please see the file COPYING in the source
# distribution of this software for license terms.
PGM="`basename $0`"
USAGE="$PGM: usage: $PGM [-w <width>] [-y <yscale>] [-p <pwidth>]"

YSCALE=0.4
WIDTH=132
PREVIEW=false

while [ $# -gt 0 ]
do
    case "$1" in
    -w)
        if [ $# -le 1 ]
	then
	    echo "$USAGE" >&2
	    exit 1
	fi
	WIDTH=$2
	shift 2
	;;
    -y)
        if [ $# -le 1 ]
	then
	    echo "$USAGE" >&2
	    exit 1
	fi
	YSCALE=$2
	shift 2
	;;
    -p)
	PREVIEW=true
	PWIDTH=$2
	shift 2
	;;
    -*)
	echo "$USAGE" >&2
	exit 1
	;;
    *)
	break
	;;
    esac
done

if [ $# -gt 1 ]
then
    echo "$USAGE" >&1
    exit 1
fi

ppmtopgm |
pnmscale -xscale 1.0 -yscale "$YSCALE" |
pnmscale -width "$WIDTH" |
pnmnorm -wpercent 10 -bpercent 1 2>/dev/null |
pnmquant -nofs -meanpixel 32 2>/dev/null |
if $PREVIEW
then
    pnmscale -width $PWIDTH |
    pnmscale -xscale 1.0 -yscale `awk "END { print 1.0 / $YSCALE; }" </dev/null`
else
    pgmtoascii
fi
