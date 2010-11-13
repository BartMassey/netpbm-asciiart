# Copyright © 2010 Bart Massey
# ALL RIGHTS RESERVED
# [This program is licensed under the "MIT License"]
# Please see the file COPYING in the source
# distribution of this software for license terms.

LIBS = -lnetpbm

TARGETS = asciitopbm asciitopgm pgmtoascii

all: $(TARGETS)

asciitopbm: asciitopbm.o
	$(CC) $(CFLAGS) -o asciitopbm asciitopbm.o $(LIBS)

asciitopgm: asciitopbm
	ln -s asciitopbm asciitopgm

pgmtoascii: pgmtoascii.o
	$(CC) $(CFLAGS) -o pgmtoascii pgmtoascii.o $(LIBS)

shades.c: glyphshades
	./glypshades > shades.c

glyphshades: glyphshades.o
	$(CC) $(CFLAGS) -o glyphshades glyphshades.o -lcairo

clean:
	-rm -f *.o $(TARGETS)
