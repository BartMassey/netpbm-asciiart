/* Copyright © 2010 Bart Massey */
/* [This program is licensed under the "MIT License"]
 * Please see the file COPYING in the source
 * distribution of this software for license terms.
 */

/* List of available glyphshades font shadings */

#include "glyphshades.h"

struct shade shades[] {
    {"sans", glyphscale_sans, glyphshades_sans},
    {"typewriter", glyphscale_typewriter, glyphshades_typewriter},
    {0, 0, 0}
}
