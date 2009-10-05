#!/bin/sh
PGM="`basename $0`"
USAGE="$PGM: usage: $PGM [-w <width>] [-y <yscale>] [<jpeg-file>]"

YSCALE=0.4
WIDTH=132

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

djpeg $1 |
ppmtopgm |
pgmenhance |
pnmscale -xscale=1 -yscale="$YSCALE" |
pnmscale -width="$WIDTH" |
pnmnorm -wpercent 10 -bpercent 1 2>/dev/null |
pgmtoascii
