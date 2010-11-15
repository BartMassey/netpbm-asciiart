#!/bin/sh
# Copyright © 2010 Bart Massey
# ALL RIGHTS RESERVED
# [This program is licensed under the "MIT License"]
# Please see the file COPYING in the source
# distribution of this software for license terms.
PGM="`basename $0`"
USAGE="$PGM: usage: $PGM [-reverse] [-font <tag>] [-width <width>] [-yscale <yscale>] [-preview <width>]"

YSCALE=2.5
WIDTH=132
PREVIEW=false
REVERSE=""
FONT_TAG=""

SCALER=pnmscale
UNSCALER=pnmscale
if which pamscale >/dev/null
then
    SCALER="pamscale -filter=sinc"
    UNSCALER="pamscale -nomix"
fi

TMP=/tmp/mkasciiart.$$
trap "rm -f $TMP" 0 1 2 3 15

while [ $# -gt 0 ]
do
    case "$1" in
    -reverse)
	REVERSE="-reverse"
	shift
	;;
    -font)
        if [ $# -le 1 ]
	then
	    echo "$USAGE" >&2
	    exit 1
	fi
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
        shift 2
	;;
    -preview)
        if [ $# -le 1 ]
	then
	    echo "$USAGE" >&2
	    exit 1
	fi
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
$SCALER -xscale $YSCALE -yscale 1.0 |
$SCALER -width "$WIDTH" |
pnmnorm -quiet -wpercent 3 -bpercent 3 >$TMP
pnmquant -nofs -meanpixel 99 $TMP 2>/dev/null |
if $PREVIEW
then
    if [ "$REVERSE" != "" ] || [ "$FONT_TAG" != "" ]
    then
	echo "$PGM: warning: -preview ignores -reverse and -font-tag" >&2
    fi
    $UNSCALER -width $PWIDTH |
    $UNSCALER -xscale 1.0 -yscale $YSCALE
else
    pgmtoasciiart $REVERSE $FONT_TAG
fi
