/*
 * Copyright © 2010 Bart Massey
 * ALL RIGHTS RESERVED
 * [This program is licensed under the "MIT License"]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

#include <assert.h>
#include <pbm.h>
#include <pgm.h>
#ifndef DEBIAN
#include <shhopt.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glyphshades.h"

static int maxscale;           /* gray-levels available */
static int threshold;          /* cut-point */
static char **grays;            /* array of gray levels */
static int maxlinelen;         /* width in chars */
static int numlines;           /* height in chars */
static int screen, contrast;   /* relative to maxscale */
static int writepgm = 0;       /* true if we are writing a graymap */
static int mingray = 15;       /* if it aint white, its at least this */
static char *usage;

/* command-line arguments */
static char *progname;   /* basename of invoked program */
static FILE *fp;
static int pix_width, pix_height;
static float sscreen, scontrast;
static char *scale;
static char *font_tag;

static void parse_pbm_command_line(int argc, char ** argv) {
    optEntry *option_def = malloc(100 * sizeof(optStruct));
    optStruct3 opt;
    unsigned int option_def_index;
    int i;

    option_def_index = 0;
    OPTENT3(0, "cwidth", OPT_INT, &pix_width, NULL, 0);
    OPTENT3(0, "cheight", OPT_INT, &pix_height, NULL, 0);
    OPTENT3(0, "mesh", OPT_FLOAT, &sscreen, NULL, 0);
    OPTENT3(0, "contrast", OPT_FLOAT, &scontrast, NULL, 0);
    OPTENT3(0, "scale", OPT_STRING, &scale, NULL, 0);
    OPTENT3(0, "font", OPT_STRING, &font_tag, NULL, 0);

    pix_width = 6;
    pix_height = 15;
    sscreen = 0.01;
    scontrast = 0.70;
    scale = scalechars_sans;
    font_tag = "sans";

    opt.opt_table = option_def;
    opt.short_allowed = 0;  /* We have no short (old-fashioned) options */
    opt.allowNegNum = 1;  /* We may have parms that are negative numbers */

    optParseOptions3(&argc, argv, opt, sizeof(opt), 0);

    if (argc > 2)
        pm_error(usage);
    if (pix_width < 0)
        pm_error("-cwidth may not be negative.");
    if (pix_height < 0)
        pm_error("-cheight may not be negative.");
    for (i = 0; shades[i].font_tag; i++) {
        if (!strcmp(font_tag, shades[i].font_tag)) {
            scale = *shades[i].scale;
            break;
        }
    }
    if (!shades[i].font_tag)
        pm_error("unknown font tag %s", font_tag);
    if (argc == 2) {
        fp = fopen(argv[1], "r");
        if (!fp)
            pm_error("could not open input file");
    }
}


static void parse_pgm_command_line(int argc, char ** argv) {
    optEntry *option_def = malloc(100 * sizeof(optStruct));
    optStruct3 opt;
    unsigned int option_def_index;
    int i;

    option_def_index = 0;
    OPTENT3(0, "cwidth", OPT_INT, &pix_width, NULL, 0);
    OPTENT3(0, "cheight", OPT_INT, &pix_height, NULL, 0);
    OPTENT3(0, "scale", OPT_STRING, &scale, NULL, 0);
    OPTENT3(0, "font", OPT_STRING, &font_tag, NULL, 0);

    pix_width = 5;
    pix_height = 12;
    sscreen = 0.01;
    scontrast = 0.70;
    scale = scalechars_sans;
    font_tag = "sans";

    opt.opt_table = option_def;
    opt.short_allowed = 0;  /* We have no short (old-fashioned) options */
    opt.allowNegNum = 1;  /* We may have parms that are negative numbers */

    optParseOptions3(&argc, argv, opt, sizeof(opt), 0);

    if (argc > 2)
        pm_error(usage);
    if (pix_width < 0)
        pm_error("-cwidth may not be negative.");
    if (pix_height < 0)
        pm_error("-cheight may not be negative.");
    for (i = 0; shades[i].font_tag; i++) {
        if (!strcmp(font_tag, shades[i].font_tag)) {
            scale = *shades[i].scale;
            break;
        }
    }
    if (!shades[i].font_tag)
        pm_error("unknown font tag %s", font_tag);
    if (argc == 2) {
        fp = fopen(argv[1], "r");
        if (!fp)
            pm_error("could not open input file");
    }
}


static int clamp(int c, int m) {
    if (c < 0)
        return 0;
    if (c > m - 1)
        return m - 1;
    return c;
}

/* bilinear (2-d Gouraud) shading */
static int g2d(int x, int y) {
    int w = pix_width;
    int h = pix_height;
    int dx = x % w;
    int dy = y % h;
    int chx1 = x / w;
    int chy1 = y / h;
    int chx0, chy0;
    int g00, g01, g10, g11, g0, g1, g;
    int xs, ys;

    /* Figure out which quadrant of the cell we're in and
       thus find the corners. While we're at it, compute the
       scale factors. We work in a common denominator of 4 *
       (w - 1) * (h - 1) to keep things in integers. We are
       careful to make sure that the cell centers contain
       some of the requested color. */
    --w; --h;
    /* First x. */
    if (dx == w / 2 || dx == (w + 1) / 2) {
        chx0 = chx1;
        xs = 2 * w;
    } else if (2 * dx < w) {
        chx0 = clamp(chx1 - 1, maxlinelen);
        xs = (w + 1) + dx + 1;
    } else {
        chx0 = clamp(chx1 + 1, maxlinelen);
        xs = 2 * (w + 1) - dx;
    }
    assert(0 <= xs);
    assert(xs <= 2 * w);
    /* Then y. */
    if (dy == h / 2 || dy == (h + 1) / 2) {
        chy0 = chy1;
        ys = 2 * h;
    } else if (2 * dy < h) {
        chy0 = clamp(chy1 - 1, maxlinelen);
        ys = (h + 1) + dy + 1;
    } else {
        chy0 = clamp(chy1 + 1, maxlinelen);
        ys = 2 * (h + 1) - dy;
    }
    assert(0 <= ys);
    assert(ys <= 2 * h);

    /* Get the corner gray levels. */
    g00 = grays[chy0][chx0];
    g10 = grays[chy0][chx1];
    g01 = grays[chy1][chx0];
    g11 = grays[chy1][chx1];

    /* Return the bilinear shading on the given corners */
    /* First find the x grays for the top and bottom */
    g0 = g00 * 2 * w + (g10 - g00) * xs;
    g1 = g01 * 2 * w + (g11 - g01) * xs;
    g = g0 * 2 * h + (g1 - g0) * ys;
    return g / (4 * w * h);
}


#define RAND(N) ((N==0)?0:(random() % (N)))

/* paint a character block one "real" pixel at a time. */
static void paint_grays(gray **gp) {
    int i, j;
    int dx = pix_width * maxlinelen;
    int dy = pix_height * numlines;

    for (i=0; i < dx; i++) {
        for (j=0; j < dy; j++) {
            int v = maxscale - g2d(i, j);
            assert(v >= 0 && v <= maxscale);
            gp[j][i] = v;
        }
    }
}


/* customized halftoning for ascii images */
static int halftone(int g, int x, int y) {
    return g +                                       /* gray */
        (((x & 1) ^ (y & 1)) ? screen : -screen) +   /* screening */
        (int) (RAND( 2 * contrast ) - contrast)      /* dither */
        > threshold;                                 /* thresholding */
}
     
/* paint a character block one "real" pixel at a time. */
static void paint_bits(bit **bp) {
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
        bp[dy][i] = halftone(grays[dy / pix_height][i / pix_width], i, dy);
    for (j=0; j <= dy; j++)
        bp[j][dx] = halftone(grays[j / pix_height][dx / pix_width], dx, j);
}


/*
 * find the character pixel (chixel) in the gray-scale and return its
 * level.
 */
static int get_level(char chixel) {
    register int lev;
    static int warned = 0;
    int scalelen = strlen(scale);

    if (chixel == ' ')
        return 0;
    for (lev = 0; lev < scalelen && scale[lev] != chixel; lev++)
        /* do nothing */;

    /* if not found, then max it out. */
    if (lev >= scalelen) {
        if (!warned) {
            if (chixel >= ' ' && chixel < 127)
                pm_message("warning: unexpected char '%c' (%d) in input set to black (future warnings suppressed)",
                           chixel, chixel);
            else
                pm_message("warning: unexpected char (%d) in input set to black (future warnings suppressed)",
                           chixel);
            warned = 1;
        }
        return maxscale - 1;
    }

    return lev + mingray;
}


/*
 * my version of gets() read in a line from the file, strips
 * the newline, adjust maxlinelen if necessary, stores the
 * line, returns the number of chars (counting
 * the null) in *sp, or 0 on EOF.
 */
static int safe_gets(char **sp) {
    int targ, res;
    char *s, *si;

    targ = maxlinelen;
    si = s = malloc(targ);
    while (1) {
        if (targ <= 0) {
            targ = 40;
            res = si - s;
            s = realloc(s, res + targ);
            si = s + res;
        }
        targ--;
        res = getc(fp);
        if (res == EOF) {
            if (si > s)
                pm_message("unexpected EOF");
            *sp = 0;
            return 0;
        }
        if (res == '\n')
            break;
        if (res == '\r') {
            int ch = getc(fp);
            if (ch == EOF)
                break;
            if (ch != '\n')
                ungetc(ch, fp);
            break;
        }
        *si++ = res;
    }
    *si = '\0';
    res = si - s + 1;
    s = realloc(s, res);
    if (res > maxlinelen)
        maxlinelen = res;
    *sp = s;
    return res;
}


int main(int argc, char **argv) {
    int x, y, l;
    int maxlines = 10000;
    char **gp;

    pgm_init(&argc, argv);
    progname = rindex(argv[0], '/');
    if (!progname)
        progname = argv[0];
    else
        progname++;
    fp = stdin;
    if (!strcmp(progname, "asciiarttopgm")) {
        writepgm = 1;
        usage = "usage: asciiarttopgm [-cwidth <pixels>] [-cheight <pixels>] [-scale <string>] [-font <font-tag>] [<filename>]";
        parse_pgm_command_line(argc, argv);
    } else if (!strcmp(progname, "asciiarttopbm")) {
        writepgm = 0;
        usage = "usage: asciiarttopbm [-cwidth <pixels>] [-cheight <pixels>] [-contrast <0..1>] [-mesh <0..1>] [-scale <string>] [-font <font-tag>] [<filename>]";
        parse_pbm_command_line(argc, argv);
    } else {
        pm_error("asciiarttopbm/asciiarttopgm invoked under unknown name");
    }

    numlines = 0;
    maxscale = strlen(scale) + mingray;
    contrast = (1 - scontrast) * maxscale;
    screen = sscreen * maxscale;
    grays = (char **) malloc(maxlines * sizeof(char *));
    if (grays == 0) {
        perror(progname);
        exit(1);
    }
    gp = grays;

    while (safe_gets(gp++)) {
        numlines++;
        if (numlines >= maxlines) {
            maxlines += 10000;
            grays = realloc(grays, maxlines);
            if (!grays) {
                perror(progname);
                exit(1);
            }
            gp = grays + numlines;
        }
    }
    maxlinelen--;  /* throw away the null */

    if (numlines <= 0)
        pm_error("cannot process empty file");

    for (y = 0; y < numlines; y++) {
        if ((l = strlen(grays[y])) < maxlinelen) {
            grays[y] = realloc( grays[y], maxlinelen + 1 );
            for( x = l; x < maxlinelen; x++ )
                grays[y][x] = ' ';
            grays[y][x] = '\0';
        }
        for (x = 0; x < maxlinelen; x++)
            grays[y][x] = get_level(grays[y][x]);
    }
    for (y = 0; y < numlines; y++)
        for (x = 0; x < maxlinelen; x++)
            threshold += grays[y][x];
    threshold /= numlines * maxlinelen;

    if (writepgm) {
        gray **gp = pgm_allocarray(maxlinelen * pix_width,
                                   numlines * pix_height);
        paint_grays(gp);
        pgm_writepgm(stdout, (gray **)gp,
                     maxlinelen * pix_width,
                     numlines * pix_height,
                     (gray) maxscale, 0);
    } else {
        bit **bp = pbm_allocarray(maxlinelen * pix_width,
                                  numlines * pix_height);
        paint_bits(bp);
        pbm_writepbm(stdout, (bit **)bp,
                     maxlinelen * pix_width,
                     numlines * pix_height, 0);
    }
    return 0;
}
