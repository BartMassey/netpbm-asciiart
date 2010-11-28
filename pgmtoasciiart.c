/*
 * Copyright © 2010 Bart Massey
 * ALL RIGHTS RESERVED
 * [This program is licensed under the "MIT License"]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

#include <assert.h>
#include <pgm.h>
#ifdef DEBIAN
#define optParseOptions3 pm_optParseOptions3
#else
#include <shhopt.h>
#endif
#include <stdio.h>
#include <string.h>

#include "glyphshades.h"

static char *usage = "usage: pgmtoasciiart [-reverse] [-clamp] [-threshold]\n    [-f <font-tag>] [-tonescale] [-scale <chars>] [file]";

static int maxscale;      /* length of scale (i.e. gray-levels available) */
static int mingray = 1;   /* if it aint at least this, it's white */
static struct glyphshade *shade;    /* shade information */ 
static int gscale;        /* adjustment for very HDR inputs */

/* command-line arguments */
static FILE *fp;
static char *scale;   /* for pointing to the gray-level being used */
static char *font_tag;
static unsigned int tonescale, reverse, clamp, threshold;

static void parse_command_line(int argc, char ** argv) {
    optEntry *option_def = malloc(100 * sizeof(optStruct));
    optStruct3 opt;
    unsigned int option_def_index;
    int i;

    option_def_index = 0;
    OPTENT3(0, "scale", OPT_STRING, &scale, NULL, 0);
    OPTENT3(0, "tonescale", OPT_FLAG, NULL, &tonescale, 0);
    OPTENT3(0, "reverse", OPT_FLAG, NULL, &reverse, 0);
    OPTENT3(0, "clamp", OPT_FLAG, NULL, &clamp, 0);
    OPTENT3(0, "threshold", OPT_FLAG, NULL, &threshold, 0);
    OPTENT3(0, "font", OPT_STRING, &font_tag, NULL, 0);

    reverse = 0;
    clamp = 0;
    threshold = 0;
    fp = stdin;
    scale = 0;
    tonescale = 0;
    font_tag = "sans";
    shade = 0;

    opt.opt_table = option_def;
    opt.short_allowed = 0;  /* We have no short (old-fashioned) options */
    opt.allowNegNum = 1;  /* We may have parms that are negative numbers */

    optParseOptions3(&argc, argv, opt, sizeof(opt), 0);

    if (argc > 2)
        pm_error(usage);
    for (i = 0; shades[i].font_tag; i++) {
        if (!strcmp(font_tag, shades[i].font_tag)) {
            if (tonescale && !scale)
                scale = *shades[i].scale;
            shade = shades[i].shades;
            break;
        }
    }
    if (!shade)
        pm_error("unknown font tag %s", font_tag);
    if (argc == 2) {
        fp = fopen(argv[1], "r");
        if (!fp)
            pm_error("could not open input file");
    }
}

static char shadechar(unsigned int g, unsigned int nscale) {
    float w, frac;
    int lower, upper;
    
    g >>= gscale;
    w = g / (float) nscale;
    if (!reverse)
        w = 1.0 - w;
    if (!clamp)
        w *= shade[94].w;
    if (w >= shade[94].w)
        return shade[94].ch;
    lower = 0;
    upper = 94;
    /* XXX This could be replaced by a fast binary search,
       but how fast can it even need to be? */
    while (shade[lower + 1].w <= w)
        lower++;
    while (shade[upper - 1].w >= w)
        upper--;
    if (lower == upper)
        return shade[lower].ch;
    frac = (w - shade[lower].w) / (shade[upper].w - shade[lower].w);
    if (threshold) {
        if (frac >= 0.5)
            return shade[upper].ch;
        return shade[lower].ch;
    }
    if (random() % 100 >= 100 * frac)
        return shade[upper].ch;
    return shade[lower].ch;
}

static char scalechar(unsigned int g, unsigned int nscale) {
    int k = (maxscale - 1) * (nscale - g) / nscale;

    assert(k >= 0 && k < maxscale);
    if (reverse) {
        if (k >= maxscale - mingray)
            return ' ';
        return scale[maxscale - mingray - k];
    }
    if (k < mingray)
        return ' ';
    return scale[k - mingray];
}


static void onechar(unsigned int g, unsigned int nscale) {
    if (scale)
        putchar(scalechar(g, nscale));
    else
        putchar(shadechar(g, nscale));
}

int main(int argc, char **argv) {
    int cols, rows;
    unsigned int nscale;
    gray **gp;
    int i, j;

    pgm_init(&argc, argv);
    parse_command_line(argc, argv);

    if (scale)
        maxscale = strlen(scale) + mingray;
    gp = pgm_readpgm(fp, &cols, &rows, &nscale);
    gscale = 0;
    while (nscale >= (1 << 16)) {
        nscale >>= 1;
        gscale++;
    }
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++)
            onechar(gp[i][j], nscale);
        putchar('\n');
    }
    return 0;
}
