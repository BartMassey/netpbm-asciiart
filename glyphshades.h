/* Copyright © 2010 Bart Massey */
/* [This program is licensed under the "MIT License"]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

struct glyphshade {
    char ch;
    float w;
};

struct shade {
    char *font_tag;
    char *scale;
    struct glyphshade *shades;
};

extern struct glyphshade glyphshades_sans[];
extern struct glyphshade glyphshades_typewriter[];

extern char *scalechars_sans;
extern char *scalechars_typewriter;

extern struct shade shades[];
