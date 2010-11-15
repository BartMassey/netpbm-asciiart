# Copyright © 2010 Bart Massey
# ALL RIGHTS RESERVED
# [This program is licensed under the "MIT License"]
# Please see the file COPYING in the source
# distribution of this software for license terms.

DESTDIR = /usr/local
INCLUDE = -I$(DESTDIR)/include
# Uncomment for Debian's ancient netpbm-dev
#DEFINE = -DDEBIAN -DoptParseOptions3=pm_optParseOptions3
CDEBUG = -O2
CFLAGS = $(CDEBUG) -Wall $(INCLUDE) $(DEFINE)
LIB = -L$(DESTDIR)/lib -lnetpbm

PACKAGE = netpbm-asciiart

INSTALL = install

SRC = asciiarttopbm.c pgmtoasciiart.c mkasciiart.sh \
      glyphshades.c glyphshades.h shades.c

MISC = Makefile COPYING README TODO mkdist.sh

TARGETS = asciiarttopbm pgmtoasciiart
MANPAGES = asciiarttopbm.1 asciiarttopgm.1 pgmtoasciiart.1 mkasciiart.1

SHADESRC = shades_typewriter.c shades_sans.c scale_sans.c scale_typewriter.c
SHADEOBJ = shades_typewriter.o shades_sans.o scale_sans.o scale_typewriter.o

all: $(TARGETS)

asciiarttopbm: asciiarttopbm.o shades.o $(SHADEOBJ) glyphshades.h
	$(CC) $(CFLAGS) -o asciiarttopbm \
	    asciiarttopbm.o shades.o $(SHADEOBJ) $(LIB)

pgmtoasciiart: pgmtoasciiart.o shades.o $(SHADEOBJ) glyphshades.h
	$(CC) $(CFLAGS) -o pgmtoasciiart \
	    pgmtoasciiart.o shades.o $(SHADEOBJ) $(LIB)

$(SHADEOBJ): glyphshades.h

#NONDIST

# This stuff doesn't go in the tarball makefile, as it depends
# on glyphshades, which depends on Cairo.  Instead, these C
# files are prebuilt and included in the dist.

shades_typewriter.c: glyphshades
	./glyphshades -m struct -f 'Courier New' typewriter \
	    >shades_typewriter.c

shades_sans.c: glyphshades
	./glyphshades -m struct -f 'Bitstream Vera Sans Mono' sans \
	    >shades_sans.c

scale_typewriter.c: glyphshades
	./glyphshades -m scale -f 'Courier New' typewriter \
	    >scale_typewriter.c

scale_sans.c: glyphshades
	./glyphshades -m scale -f 'Bitstream Vera Sans Mono' sans \
	    >scale_sans.c

glyphshades: glyphshades.o
	$(CC) $(CFLAGS) -o glyphshades glyphshades.o -lcairo

#ENDNONDIST

install: $(TARGETS) mkasciiart.sh
	for i in $(TARGETS); do $(INSTALL) $$i $(DESTDIR)/bin ; done
	cd $(DESTDIR)/bin && \
	    rm -rf asciiarttopgm && \
	    ln -s asciiarttopbm asciiarttopgm
	$(INSTALL) mkasciiart.sh $(DESTDIR)/bin/mkasciiart
	for i in $(MANPAGES); do \
	    $(INSTALL) -m 644 $$i $(DESTDIR)/man/man1 ; done

clean:
	-rm -f *.o $(TARGETS) glyphshades
	-rm -rf netpbm-asciiart netpbm-asciiart.tgz

distclean: clean
	-rm -f $(SHADESRC)

dist: $(SHADESRC)
	sh -x mkdist.sh $(PACKAGE) $(SRC) $(SHADESRC) $(MISC)
