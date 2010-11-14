# Copyright © 2010 Bart Massey
# ALL RIGHTS RESERVED
# [This program is licensed under the "MIT License"]
# Please see the file COPYING in the source
# distribution of this software for license terms.

DESTDIR=/usr/local

INSTALL=install

LIBS = -lnetpbm

TARGETS = asciiarttopbm pgmtoasciiart
MANPAGES = asciiarttopbm.1 asciiarttopgm.1 pgmtoasciiart.1

SHADESRC = shades_typewriter.c shades_sans.c scale_sans.c scale_typewriter.c
SHADEOBJ = shades_typewriter.o shades_sans.o scale_sans.o scale_typewriter.o

all: $(TARGETS)

asciiarttopbm: asciiarttopbm.o shades.o $(SHADEOBJ) glyphshades.h
	$(CC) $(CFLAGS) -o asciiarttopbm asciiarttopbm.o shades.o $(SHADEOBJ) $(LIBS)

pgmtoasciiart: pgmtoasciiart.o shades.o $(SHADEOBJ) glyphshades.h
	$(CC) $(CFLAGS) -o pgmtoasciiart pgmtoasciiart.o shades.o $(SHADEOBJ) $(LIBS)

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

install: $(TARGETS) mkasciiart.sh
	for i in $(TARGETS); do $(INSTALL) $$i $(DESTDIR)/bin ; done
	cd $(DESTDIR)/bin && rm -rf asciiarttopgm && ln -s asciiarttopbm asciiarttopgm
	$(INSTALL) mkasciiart.sh $(DESTDIR)/bin/mkasciiart
	for i in $(MANPAGES); do $(INSTALL) -m 644 $$i $(DESTDIR)/man/man1 ; done

clean:
	-rm -f *.o $(TARGETS) $(SHADESRC) glyphshades
