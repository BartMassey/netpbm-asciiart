/*
 * Copyright © 2010 Bart Massey
 * ALL RIGHTS RESERVED
 * [This program is licensed under the "MIT License"]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pgm.h"
#include "pbm.h"

/*
 * For best efficiency on machines which support it,
 * use -DINLINE=inline.  If you can't do this, your
 * compiler had better do reasonable register allocation.
 * Otherwise, you should rewrite some of this as a series of
 * macros.
 */
#ifndef INLINE
#define INLINE	/*inline*/
#endif

/* the default 64 gray-level */
char *scalechars = " .:,`~-^!;|/[*(}?+7<i=IrlvYC30Otu9ZVGDa&8pwdhRFUgHXqbAkQNKEBmWM#";
char *scale;		/* for pointing to the gray-level being used */
int maxscale;		/* length of scale (i.e. gray-levels available) */
int threshold;		/* cut-point */
char **grey;		/* array of grey levels */
char *gp;		/* array index */
FILE *fp;	        /* input file */
int maxlinelen;		/* width in chars */
int numlines;		/* height in chars */
int pix_width = 4;	/* char width in pixels */
int pix_height = 6;	/* char height in pixels */
double sscreen = 0.01;  /* scaled screen value */
double scontrast = 0.70;  /* scaled contrast value */
int screen, contrast;	/* relative to maxscale */
char *progname;         /* basename of invoked program */
int writepgm = 0;       /* true if we are writing a greymap */

/* bilinear (2-d Gouraud) shading */
INLINE int g2d( x, y )
int x, y;  /* pixel coords */
{
	int dx = x % pix_width;
	int dy = y % pix_height;
	int chx = x / pix_width;
	int chy = y / pix_height;
	int g00 = grey[ chy ][ chx ];
	int g10 = grey[ chy ][ chx + 1 ];
	int g01 = grey[ chy + 1 ][ chx ];
	int g11 = grey[ chy + 1 ][ chx + 1 ];

	return
	    (g00 * pix_width * pix_height +
	    (g10 - g00) * dx * pix_height +
	    (g01 - g00) * dy * pix_width +
	    (g00 - g10 - g01 +g11) * dx * dy) /
		(pix_width * pix_height);
}


#define RAND(N) ((N==0)?0:(random() % (N)))

/* paint a character block one "real" pixel at a time. */
void
paint_grays( gp )
gray **gp;
{
	int i, j;
	int dx = pix_width * (maxlinelen - 1);
	int dy = pix_height * (numlines - 1);

	for (i=0; i < dx; i++)
	    for (j=0; j < dy; j++) {
		int v = maxscale - g2d(i, j);
		assert(v >= 0 && v <= maxscale);
		gp[j][i] = v;
	    }
	for (i=0; i < dx; i++)
	    gp[dy][i] = grey[dy / pix_height][i / pix_width];
	for (j=0; j <= dy; j++)
	    gp[j][dx] = grey[j / pix_height][dx / pix_width];
}


/* customized halftoning for ascii images */
int
halftone(g, x, y)
int g, x, y;
{
    return g +                                       /* gray */
	(((x & 1) ^ (y & 1)) ? screen : -screen) +   /* screening */
	(int) (RAND( 2 * contrast ) - contrast)      /* dither */
	> threshold;                                 /* thresholding */
}
     
/* paint a character block one "real" pixel at a time. */
void
paint_bits( bp )
bit **bp;
{
	int i, j;
	int dx = pix_width * (maxlinelen - 1);
	int dy = pix_height * (numlines - 1);

	for (i=0; i < dx; i++)
	    for (j=0; j < dy; j++)
		if (halftone(g2d(i, j), i, j))
		    bp[j][i] = 1;
	        else
		    bp[j][i] = 0;
	for (i=0; i < dx; i++)
	    bp[dy][i] = halftone(grey[dy / pix_height][i / pix_width], i, dy);
	for (j=0; j <= dy; j++)
	    bp[j][dx] = halftone(grey[j / pix_height][dx / pix_width], dx, j);
}



/*
 * find the character pixel (chixel) in the gray-scale and return its
 * level.
 */
int
get_level(chixel)
char chixel;
{
	register int lev;
	static int warned = 0;

	for (lev=0; lev < maxscale && scale[lev] != chixel; lev++)
		;

	/* if not found, then max it out. */
	if (lev > (maxscale-1)) {
		lev = maxscale -1;
		if( !warned ) {
			fprintf( stderr,
				 "%s: warning: unexpected char '%c' (%d) in input (future warnings suppressed)\n",
				 progname, chixel, chixel );
			warned = 1;
		}
	}

	return(lev);
}


/*
 * my version of gets() read in a line from the file, strips
 * the newline, adjust maxlinelen if necessary, stores the
 * line, returns the number of chars (counting
 * the null) in *sp, or 0 on EOF.
 */
int
safe_gets(sp)
char **sp;
{
	int targ, res;
	char *s, *si;

	targ = maxlinelen;
	si = s = malloc( targ );
	while( 1 ) {
		if( targ <= 0 ) {
			targ = 40;
			res = si - s;
			s = realloc( s, res + targ );
			si = s + res;
		}
		targ--;
		res = getc( fp );
		if( res == EOF ) {
			if( si > s )
				fprintf( stderr, "%s: unexpected EOF\n", progname );
			*sp = 0;
			return 0;
		}
		if( res == '\n' )
			break;
		*si++ = res;
	}
	*si = '\0';
	res = si - s + 1;
	s = realloc( s, res );
	if ( res > maxlinelen )
		maxlinelen = res;
	*sp = s;
	return res;
}


int main(argc, argv)
int argc;
char *argv[];
{
	int x, y, l;
	char **gp;

	pgm_init(&argc, argv);
	progname = rindex(argv[0], '/');
	if (!progname)
	    progname = argv[0];
	else
	    progname++;
	fp = stdin;
	if (!strcmp(progname, "asciitopgm"))
	    writepgm = 1;
	else if (!strcmp(progname, "asciitopbm"))
	    writepgm = 0;
	else
	    goto usage;
	while( argc > 1 && argv[1][0] == '-' ) {
		switch( argv[1][1] ) {
		case 'h':
		    goto usage;
		}
		if( (argv[1][1] && argv[1][2]) || argc < 3 )
			goto usage;
		switch( argv[1][1] ) {
		case 'm':
			sscreen = atof( argv[2] );
			break;
		case 'c':
			scontrast = atof( argv[2] );
			break;
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
#if 0
	if (argc != 1)
	    goto usage;
#endif

	numlines = 0;
	scale = scalechars;
	maxscale = strlen( scale );
	contrast = (1 - scontrast) * maxscale;
	screen = sscreen * maxscale;
	grey = (char **) malloc( 10000 * sizeof(char *) );  /* XXX */
	gp = grey;

	while( safe_gets( gp++ ) )
		numlines++;
	maxlinelen--;  /* throw away the null */

	for( y = 0; y < numlines; y++ ) {
		if( (l = strlen( grey[y] )) < maxlinelen ) {
			grey[y] = realloc( grey[y], maxlinelen + 1 );
			for( x = l; x < maxlinelen; x++ )
				grey[y][x] = ' ';
			grey[y][x] = '\0';
		}
		for( x = 0; x < maxlinelen; x++ )
			grey[y][x] = get_level( grey[y][x] );
	}
	for( y = 0; y < numlines; y++ )
		for( x = 0; x < maxlinelen; x++ )
			threshold += grey[y][x];
	threshold /= numlines * maxlinelen;

	if (writepgm) {
	    gray **gp = pgm_allocarray( maxlinelen * pix_width,
					numlines * pix_height );
	    paint_grays( gp );
	    pgm_writepgm( stdout, (gray **) gp,
			  maxlinelen * pix_width,
			  numlines * pix_height,
			  (gray) maxscale, 0 );
	} else {
	    bit **bp = pbm_allocarray( maxlinelen * pix_width,
					numlines * pix_height );
	    paint_bits( bp );
	    pbm_writepbm( stdout, (bit **) bp,
			  maxlinelen * pix_width,
			  numlines * pix_height, 0 );
	}
	exit(0);
usage:
	fprintf( stderr, "%s: usage: [asciitopbm|asciitopgm] [-c contrast] [-m mesh] [-s scale] [file]\n", progname );
	exit(1);
	/*NOTREACHED*/
}
