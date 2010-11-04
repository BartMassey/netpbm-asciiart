#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "pgm.h"

/* the default 32 gray-level */
char *scalechars = " .`,-~:;^!|/(<+*[}?7Y3O0C98BWM&#";
char *scale;		/* for pointing to the gray-level being used */
int maxscale;		/* length of scale (i.e. gray-levels available) */
char *progname;         /* basename of invoked program */

int main(argc, argv)
int argc;
char *argv[];
{
	int cols, rows, nscale;
	FILE *fp;
	gray **gp;
	int i, j;

	pgm_init(&argc, argv);
	progname = rindex(argv[0], '/');
	if (!progname)
	    progname = argv[0];
	else
	    progname++;
	fp = stdin;
	while( argc > 1 && argv[1][0] == '-' ) {
		switch( argv[1][1] ) {
		case 'h':
		    goto usage;
		}
		if( (argv[1][1] && argv[1][2]) || argc < 3 )
			goto usage;
		switch( argv[1][1] ) {
		case 's':
			scalechars = argv[2];
			break;
		default:
			goto usage;
		}
		argv += 2;
		argc -= 2;
	}
	if( argc == 2 ) {
		fp = fopen( argv[1], "r" );
		if( !fp ) {
			perror( "fopen" );
			exit( 1 );
		}
		argc--;
	}
	if (argc != 1)
	    goto usage;

	scale = scalechars;
	maxscale = strlen( scale );
	gp = pgm_readpgm(fp, &cols, &rows, &nscale);
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			int k = (maxscale - 1) * (nscale - gp[i][j]) / nscale;
			assert(k >= 0 && k < maxscale);
			putchar(scale[k]);
		}
		putchar('\n');
	}
	exit(0);
usage:
	fprintf( stderr, "%s: usage: pgmtoascii [-s scale] [file]\n", progname );
	exit(1);
	/*NOTREACHED*/
}
