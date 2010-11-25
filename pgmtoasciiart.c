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

static char *usage = "usage: pgmtoasciiart [-reverse] [-f <font-tag>] [-scale <chars>] [file]";

static int maxscale;      /* length of scale (i.e. gray-levels available) */
static int mingray = 1;   /* if it aint at least this, it's white */

/* command-line arguments */
static FILE *fp;
static char *scale;   /* for pointing to the gray-level being used */
static char *font_tag;
static unsigned int reverse;

static char *strreverse(char *s) {
    int n = strlen(s);
    char *t = malloc(n + 1);
    int i;

    if (!t)
        pm_error("out of memory");
    for (i = 0; i < n; i++)
        t[i] = s[n - i - 1];
    t[n] = '\0';
    return t;
}

static void parse_command_line(int argc, char ** argv) {
    optEntry *option_def = malloc(100 * sizeof(optStruct));
    optStruct3 opt;
    unsigned int option_def_index;
    int i;

    option_def_index = 0;
    OPTENT3(0, "scale", OPT_STRING, &scale, NULL, 0);
    OPTENT3(0, "reverse", OPT_FLAG, NULL, &reverse, 0);
    OPTENT3(0, "font", OPT_STRING, &font_tag, NULL, 0);

    reverse = 0;
    fp = stdin;
    scale = scalechars_sans;
    font_tag = "sans";

    opt.opt_table = option_def;
    opt.short_allowed = 0;  /* We have no short (old-fashioned) options */
    opt.allowNegNum = 1;  /* We may have parms that are negative numbers */

    optParseOptions3(&argc, argv, opt, sizeof(opt), 0);

    if (argc > 2)
        pm_error(usage);
    for (i = 0; shades[i].font_tag; i++) {
        if (!strcmp(font_tag, shades[i].font_tag)) {
            scale = *shades[i].scale;
            break;
        }
    }
    if (!shades[i].font_tag)
        pm_error("unknown font tag %s", font_tag);
    if (reverse)
        scale = strreverse(scale);
    if (argc == 2) {
        fp = fopen(argv[1], "r");
        if (!fp)
            pm_error("could not open input file");
    }
}

int main(int argc, char **argv) {
    int cols, rows;
    unsigned int nscale;
    gray **gp;
    int i, j;

    pgm_init(&argc, argv);
    parse_command_line(argc, argv);

    maxscale = strlen(scale) + mingray;
    gp = pgm_readpgm(fp, &cols, &rows, &nscale);
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            int k = (maxscale - 1) * (nscale - gp[i][j]) / nscale;
            assert(k >= 0 && k < maxscale);
            if (!reverse) {
                if (k < mingray)
                    putchar(' ');
                else
                    putchar(scale[k - mingray]);
            } else {
                if (k >= maxscale - mingray)
                    putchar(' ');
                else
                    putchar(scale[k]);
            }
        }
        putchar('\n');
    }
    return 0;
}
