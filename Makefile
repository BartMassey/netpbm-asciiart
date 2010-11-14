# Copyright © 2010 Bart Massey
# ALL RIGHTS RESERVED
# [This program is licensed under the "MIT License"]
# Please see the file COPYING in the source
# distribution of this software for license terms.

LIBS = -lnetpbm

TARGETS = asciitopbm asciitopgm pgmtoascii

SHADES = shades_typewriter.c shades_sans.c

all: $(TARGETS) $(SHADES)

asciitopbm: asciitopbm.o
	$(CC) $(CFLAGS) -o asciitopbm asciitopbm.o $(LIBS)

asciitopgm: asciitopbm
	ln -s asciitopbm asciitopgm

pgmtoascii: pgmtoascii.o
	$(CC) $(CFLAGS) -o pgmtoascii pgmtoascii.o $(LIBS)

shades_typewriter.c: glyphshades
	./glyphshades -m struct -f 'Courier New' typewiter > shades_typewriter.c

shades_typewriter.o: glyphshades.h

shades_sans.c: glyphshades
	./glyphshades -m struct -f 'Bitstream Vera Sans Mono' sans > shades_sans.c

shades_sans.o: glyphshades.h

glyphshades: glyphshades.o
	$(CC) $(CFLAGS) -o glyphshades glyphshades.o -lcairo

clean:
	-rm -f *.o $(TARGETS) $(SHADES)
