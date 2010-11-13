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

/* the default 32 gray-level */
/* black-on-white tonal scale chosen by glyphshades */
char *scale_bow = " `-',:~;^/*(+?LifzF%ke2$bm08N#M@";
/* white-on-black tonal scale is reverse of black-on-white */
char *scale_wob = "@M#N80mb$2ek%FzfiL?+(*/^;~:,'-` ";
char *scale;             /* for pointing to the gray-level being used */
int maxscale;            /* length of scale (i.e. gray-levels available) */
char *progname;          /* basename of invoked program */

int main(int argc, char **argv) {
    int cols, rows;
    unsigned int nscale;
    FILE *fp;
    gray **gp;
    int i, j;

    scale = scale_bow;
    pgm_init(&argc, argv);
    progname = rindex(argv[0], '/');
    if (!progname)
        progname = argv[0];
    else
        progname++;
    fp = stdin;
    if (argc > 0) {
        argv++;
        argc--;
    }
    while (argc > 0 && argv[0][0] == '-') {
        if (argv[0][1] == '\0' || argv[0][2] != '\0')
            goto usage;
        switch (argv[0][1]) {
        case 'h':
            goto usage;
        case 'r':
            scale = scale_wob;
            argv++;
            argc--;
            break;
        case 's':
            scale = argv[2];
            argv += 2;
            argc -= 2;
            break;
        default:
            goto usage;
        }
    }
    if (argc == 1) {
        fp = fopen(argv[1], "r");
        if (!fp) {
            perror("fopen");
            exit(1);
        }
        argc--;
    }
    if (argc != 0)
        goto usage;

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
    exit(0);
 usage:
    fprintf(stderr, "%s: usage: pgmtoascii [-r] [-s scale] [file]\n",
            progname);
    exit(1);
    /*NOTREACHED*/
}
