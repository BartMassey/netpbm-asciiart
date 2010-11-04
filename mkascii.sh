#!/bin/sh
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
    pnmscale -xscale 1.0 -yscale 2.5
else
    pgmtoascii
fi
