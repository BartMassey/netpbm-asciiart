#!/bin/sh
# Copyright © 2010 Bart Massey
# ALL RIGHTS RESERVED
# [This program is licensed under the "MIT License"]
# Please see the file COPYING in the source
# distribution of this software for license terms.
PGM="`basename $0`"
USAGE="$PGM: usage: $PGM [-reverse] [-font <tag>] [-width <width>] [-yscale <yscale>] [-preview <width>]"

YSCALE=2.5
INVYSCALE=0.4
WIDTH=132
PREVIEW=false
REVERSE=""
FONT_TAG=""

while [ $# -gt 0 ]
do
    case "$1" in
    -reverse)
	REVERSE="-reverse"
	shift
	;;
    -font)
	FONT_TAG="-font $2"
	shift 2
	;;
    -width)
        if [ $# -le 1 ]
	then
	    echo "$USAGE" >&2
	    exit 1
	fi
	WIDTH=$2
	shift 2
	;;
    -yscale)
        if [ $# -le 1 ]
	then
	    echo "$USAGE" >&2
	    exit 1
	fi
	YSCALE=$2
        INVYSCALE=`awk "END { print 1.0 / $YSCALE; }" </dev/null`
        shift 2
	;;
    -preview)
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
pnmscale -xscale 1.0 -yscale $INVYSCALE |
pnmscale -width "$WIDTH" |
pnmnorm -quiet -wpercent 3 -bpercent 3 |
pnmquant -quiet -nofs -meanpixel 99 |
if $PREVIEW
then
    pnmscale -width $PWIDTH |
    pnmscale -quiet -xscale 1.0 -yscale $YSCALE
else
    pgmtoasciiart $REVERSE $FONT_TAG
fi
