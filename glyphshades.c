/* Copyright © 2010 Bart Massey */
/* [This program is licensed under the "MIT License"]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

/* Get black density of glyphs */

#include <assert.h>
#include <cairo/cairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int popcount(unsigned char ch) {
    unsigned char acc = 0;
    int i;
    
    for(i = 0; i < 4; i++) {
        acc += ch & 0x11;
        ch >>= 1;
    }
    return (acc >> 4) + (acc & 0xf);
}

int glyphshade(cairo_t *cr, char ch, double inkwidth, double inkheight) {
    char s[2];
    cairo_surface_t *su;
    unsigned char *sd;
    int stride;
    int t = 0;
    int x, y;
#ifdef DEBUG_TRAVERSE
    unsigned char z;
#endif

    s[0] = ch;
    s[1] = '\0';
    cairo_save (cr);
    cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
    cairo_paint (cr);
    cairo_restore (cr);
    cairo_show_text(cr, s);
    su = cairo_get_target(cr);
    assert(su);
    sd = cairo_image_surface_get_data(su);
    assert(sd);
    stride = cairo_image_surface_get_stride(su);
    for (y = 0; y < inkheight; y++) {
        for (x = 0; x < inkwidth / 8; x++) {
            int c = popcount(sd[x]);
#ifdef DEBUG_TRAVERSE
            int ct = 0;
            for (z = 1; z > 0; z <<= 1) {
                if (sd[x] & z) {
                    printf("*");
                    ct++;
                } else {
                    printf(" ");
                }
            }
            assert(ct == c);
#endif
            t += c;
        }
#ifdef DEBUG_TRAVERSE
        printf("\n");
#endif
        sd += stride;
    }
    return t;
}

void usage(void) {
    fprintf(stderr, "glyphshades: usage: glyphshades [-m \"scale\" | -m \"struct\"] [-f <font-name> <font-tag>] [-s <font-size>]\n");
    exit(1);
}

struct glyphshades {
    char ch;
    float gw;
} glyphshades[95];



int cgs(const void *g1, const void *g2) {
    float gw = ((struct glyphshades *)g1)->gw - ((struct glyphshades *)g2)->gw;
    if (gw < 0)
        return -1;
    if (gw == 0)
        return 0;
    return 1;
}

int main(int argc, char **argv) {
    char *mode = 0;
    char *font_name = "Bitstream Vera Sans Mono";
    char *font_tag = "sans";
    int font_size = 48;
    cairo_surface_t *su =
        cairo_image_surface_create (CAIRO_FORMAT_A1, 100, 100);
    cairo_t *cr;
    cairo_font_extents_t fe;
    double inkheight, inkwidth;
    int gw;
    unsigned char ch;

    if (argc > 0) {
        --argc;
        argv++;
    }
    while (argc >= 2) {
        if (argc >= 3 && !strcmp(argv[0], "-f")) {
            font_name = argv[1];
            font_tag = argv[2];
            argc -= 3;
            argv += 3;
            continue;
        }
        if (!strcmp(argv[0], "-s")) {
            font_size = atoi(argv[1]);
            if (font_size <= 0) {
                fprintf(stderr, "glyphshades: invalid font size\n");
                exit(1);
            }
            argc -= 2;
            argv += 2;
            continue;
        }
        if (!strcmp(argv[0], "-m")) {
            mode = argv[1];
            argc -= 2;
            argv += 2;
            continue;
        }
        usage();
    }
    if (argc == 1)
        usage();
    assert(su);
    cr = cairo_create (su);
    assert(cr);
    cairo_select_font_face (cr, font_name,
                            CAIRO_FONT_SLANT_NORMAL,
                            CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 48);
    cairo_set_source_rgba (cr, 0.0, 0.0, 0.0, 1.0);
    cairo_font_extents(cr, &fe);
    inkheight = fe.ascent + fe.descent;
    inkwidth = fe.max_x_advance;

    if (inkwidth > 100 || inkheight > 100) {
        fprintf(stderr, "font too wide (%g > 100) or tall (%g > 100)\n",
                inkwidth, inkheight);
        exit(1);
    }
    for (ch = 32; ch < 127; ch++) {
        cairo_move_to (cr, 0.0, fe.ascent);
        gw = glyphshade(cr, ch, inkwidth, inkheight);
        glyphshades[ch - 32].ch = ch;
        glyphshades[ch - 32].gw = gw;
    }
    qsort(glyphshades, 95, sizeof(struct glyphshades), cgs);
    assert(glyphshades[0].ch == ' ');
    if (mode == 0 || !strcmp(mode, "scale")) {
        printf("/* XXX Output produced automatically by glyphshades. */\n");
        printf("#include \"glyphshades.h\"\n");
        printf("/* Scale for %s. */\n", font_name);
        printf("char *scalechars_%s = \"", font_tag);
        for (ch = 1; ch < 95; ch++) {
            char ch_str[3];
            ch_str[0] = glyphshades[ch].ch;
            ch_str[1] = '\0';
            switch(glyphshades[ch].ch) {
            case '"':
                ch_str[0] = '\\';
                ch_str[1] = '"';
                ch_str[2] = '\0';
                break;
            case '\\':
                ch_str[0] = '\\';
                ch_str[1] = '\\';
                ch_str[2] = '\0';
                break;
            }
            printf("%s", ch_str);
        }
        printf("\";\n");
    } else if (!strcmp(mode, "struct")) {
        printf("/* XXX Output produced automatically by glyphshades. */\n");
        printf("#include \"glyphshades.h\"\n");
        printf("/* Fractional glyph weights for %s.\n", font_name);
        printf("   Fractions assume default leading. */\n");
        printf("struct glyphshade glyphshades_%s[95] = {\n", font_tag);
        for (ch = 0; ch < 95; ch++) {
            char ch_str[3];
            float ch_gw;
            ch_str[0] = glyphshades[ch].ch;
            ch_str[1] = '\0';
            switch(glyphshades[ch].ch) {
            case '\'':
                ch_str[0] = '\\';
                ch_str[1] = '\'';
                ch_str[2] = '\0';
                break;
            case '\\':
                ch_str[0] = '\\';
                ch_str[1] = '\\';
                ch_str[2] = '\0';
                break;
            }
            ch_gw = glyphshades[ch].gw / (inkwidth * fe.height);
            printf("    {'%s', %.5f},\n", ch_str, ch_gw);
        }
        printf("};\n");
    }
    exit(0);
}
