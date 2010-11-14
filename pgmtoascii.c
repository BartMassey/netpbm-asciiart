/*
 * Copyright © 2010 Bart Massey
 * ALL RIGHTS RESERVED
 * [This program is licensed under the "MIT License"]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

#include <assert.h>
#include <pgm.h>
#include <stdio.h>
#include <string.h>

#include "glyphshades.h"

char *usage = "usage: pgmtoascii [-reverse] [-f <font-tag>] [-scale <chars>] [file]";

int maxscale;            /* length of scale (i.e. gray-levels available) */
char *progname;          /* basename of invoked program */
int mingrey = 1;         /* if it aint at least this, it's white */

/* command-line arguments */
FILE *fp;
char *scale;             /* for pointing to the gray-level being used */
char *font_tag;
int reverse;

static void strreverse(char *s) {
    int n = strlen(s);
    int i;
    
    for (i = 0; i < strlen(s) / 2; i++) {
        char tmp = s[i];
        s[i] = s[n - i - 1];
        s[n - i - 1] = tmp;
    }
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
    opt.short_allowed = FALSE;  /* We have no short (old-fashioned) options */
    opt.allowNegNum = TRUE;  /* We may have parms that are negative numbers */

    pm_optParseOptions3(&argc, argv, opt, sizeof(opt), 0);

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
        strreverse(scale);
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

    maxscale = strlen(scale) + mingrey;
    gp = pgm_readpgm(fp, &cols, &rows, &nscale);
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            int k = (maxscale - 1) * (nscale - gp[i][j]) / nscale;
            assert(k >= 0 && k < maxscale);
            if (!reverse) {
                if (k < mingrey)
                    putchar(' ');
                else
                    putchar(scale[k - mingrey]);
            } else {
                if (k >= maxscale - mingrey)
                    putchar(' ');
                else
                    putchar(scale[k]);
            }
        }
        putchar('\n');
    }
    return 0;
}
