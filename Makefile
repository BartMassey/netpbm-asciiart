# Copyright © 2010 Bart Massey
# ALL RIGHTS RESERVED
# [This program is licensed under the "MIT License"]
# Please see the file COPYING in the source
# distribution of this software for license terms.

LIBS = -lnetpbm

TARGETS = asciitopbm asciitopgm pgmtoascii

SHADESRC = shades_typewriter.c shades_sans.c scale_sans.c scale_typewriter.c
SHADEOBJ = shades_typewriter.o shades_sans.o scale_sans.o scale_typewriter.o

all: $(TARGETS) $(SHADESRC)

asciitopbm: asciitopbm.o shades.o $(SHADEOBJ) glyphshades.h
	$(CC) $(CFLAGS) -o asciitopbm asciitopbm.o shades.o $(SHADEOBJ) $(LIBS)

asciitopgm: asciitopbm
	ln -s asciitopbm asciitopgm

pgmtoascii: pgmtoascii.o
	$(CC) $(CFLAGS) -o pgmtoascii pgmtoascii.o $(LIBS)

shades_typewriter.c: glyphshades
	./glyphshades -m struct -f 'Courier New' typewriter > shades_typewriter.c

shades_typewriter.o: glyphshades.h

shades_sans.c: glyphshades
	./glyphshades -m struct -f 'Bitstream Vera Sans Mono' sans > shades_sans.c

shades_sans.o: glyphshades.h

scale_typewriter.c: glyphshades
	./glyphshades -m scale -f 'Courier New' typewriter > scale_typewriter.c

scale_typewriter.o: glyphshades.h

scale_sans.c: glyphshades
	./glyphshades -m scale -f 'Bitstream Vera Sans Mono' sans > scale_sans.c

scale_sans.o: glyphshades.h

glyphshades: glyphshades.o
	$(CC) $(CFLAGS) -o glyphshades glyphshades.o -lcairo

clean:
	-rm -f *.o $(TARGETS) $(SHADESRC)
