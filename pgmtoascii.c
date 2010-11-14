/*
 * Copyright © 2010 Bart Massey
 * ALL RIGHTS RESERVED
 * [This program is licensed under the "MIT License"]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "pgm.h"

char *usage = "usage: pgmtoascii [-r] [-s scale] [file]";

/* the default 32 gray-level */
/* black-on-white tonal scale chosen by glyphshades */
char *scale_bow = " `-',:~;^/*(+?LifzF%ke2$bm08N#M@";
/* white-on-black tonal scale is reverse of black-on-white */
char *scale_wob = "@M#N80mb$2ek%FzfiL?+(*/^;~:,'-` ";
int maxscale;            /* length of scale (i.e. gray-levels available) */
char *progname;          /* basename of invoked program */

/* command-line arguments */
FILE *fp;
char *scale;             /* for pointing to the gray-level being used */

static void parse_command_line(int argc, char ** argv) {
    int reverse = 0;
    optEntry *option_def = malloc(100 * sizeof(optStruct));
    optStruct3 opt;
    unsigned int option_def_index;

    option_def_index = 0;
    OPTENT3(0, "scale", OPT_STRING, &scale, NULL, 0);
    OPTENT3(0, "reverse", OPT_FLAG, NULL, &reverse, 0);

    fp = stdin;
    scale = scale_bow;

    opt.opt_table = option_def;
    opt.short_allowed = FALSE;  /* We have no short (old-fashioned) options */
    opt.allowNegNum = TRUE;  /* We may have parms that are negative numbers */

    pm_optParseOptions3(&argc, argv, opt, sizeof(opt), 0);

    if (reverse)
        scale = scale_wob;
    if (argc > 2)
        pm_error(usage);
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

    maxscale = strlen(scale);
    gp = pgm_readpgm(fp, &cols, &rows, &nscale);
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            int k = (maxscale - 1) * (nscale - gp[i][j]) / nscale;
            assert(k >= 0 && k < maxscale);
            putchar(scale[k]);
        }
        putchar('\n');
    }
    return 0;
}
