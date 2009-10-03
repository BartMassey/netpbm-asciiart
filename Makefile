LIBS =  -lnetpbm -lnetpgm

TARGETS = asciitopbm asciitopgm pgmtoascii

all: $(TARGETS)

asciitopbm: asciitopbm.o
	$(CC) $(CFLAGS) -o asciitopbm asciitopbm.o $(LIBS)

asciitopgm: asciitopbm
	ln -s asciitopbm asciitopgm

pgmtoascii: pgmtoascii.o
	$(CC) $(CFLAGS) -o pgmtoascii pgmtoascii.o $(LIBS)

clean:
	-rm -f *.o $(TARGETS)
