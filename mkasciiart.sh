#!/bin/sh
# Copyright © 2010 Bart Massey
# ALL RIGHTS RESERVED
# [This program is licensed under the "MIT License"]
# Please see the file COPYING in the source
# distribution of this software for license terms.
PGM="`basename $0`"
USAGE="$PGM: usage: ... | $PGM [-width <width>] [-yscale <yscale>]\
    [-preview <width>] [-norm] [-rotate] [<pgmtoascii-args>]"

YSCALE=2.5
WIDTH=132
PREVIEW=false
REVERSE=""
FONT_TAG=""
NORM=false

SCALER=pnmscale
UNSCALER=pnmscale
if which pamscale >/dev/null
then
    SCALER="pamscale -filter=sinc"
    UNSCALER="pamscale -nomix"
fi

ROTATE=false

TMP=/tmp/mkasciiart.$$
trap "rm -f $TMP" 0 1 2 3 15

while [ $# -gt 0 ]
do
    case "$1" in
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
    -norm)
	NORM=true
	shift
	;;
    -rotate)
        ROTATE=true
        shift
        ;;
    -*)
	break
	;;
    *)
	echo "$USAGE" >&2
	exit 1
	;;
    esac
done

# ppmtopgm |
if $ROTATE
then
    pnmflip -cw
else
    cat
fi |
$SCALER -xscale $YSCALE -yscale 1.0 |
$SCALER -width "$WIDTH" |
if $NORM
then
    pnmnorm -quiet -wpercent 3 -bpercent 3
else
    cat
fi >$TMP
pnmquant -nofs -meanpixel 99 $TMP 2>/dev/null |
if $PREVIEW
then
    if [ "$*" != "" ]
    then
	echo "$PGM: extra args to -preview" >&2
	exit 1
    fi
    $UNSCALER -width "$PWIDTH" |
    $UNSCALER -xscale 1.0 -yscale "$YSCALE"
else
    pgmtoasciiart "$@"
fi
