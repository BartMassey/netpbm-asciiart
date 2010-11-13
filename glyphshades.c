/* Copyright © 2010 Bart Massey
   Get black density of glyphs */

#include <assert.h>
#include <cairo/cairo.h>
#include <stdio.h>
#include <stdlib.h>

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
        for (x = 0; x < inkwidth / 8; x++)
            t += popcount(sd[x]);
        sd += stride;
    }
    return t;
}

int main(int argc, char **argv) {
    char *font_name = "Courier New";
    cairo_surface_t *su =
        cairo_image_surface_create (CAIRO_FORMAT_A1, 100, 100);
    cairo_t *cr;
    cairo_font_extents_t fe;
    double inkheight, inkwidth;
    int gw;
    unsigned char ch;

    switch(argc) {
    case 0:
    case 1:
        break;
    case 2:
        font_name = argv[1];
        break;
    default:
        fprintf(stderr, "glyphshades: usage: glyphshades [font]\n");
        exit(1);
    }
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
    for (ch = 33; ch < 127; ch++) {
        cairo_move_to (cr, 0.0, fe.ascent);
        gw = glyphshade(cr, ch, inkwidth, inkheight);
        printf("%c %.5f\n", ch, gw * 255.0 / (double) (inkwidth * inkheight));
    }
    exit(0);
}
