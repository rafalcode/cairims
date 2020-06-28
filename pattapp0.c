/* here we take an image and define a pattern based on the entire image
 * and then apply it to another surface. */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cairo/cairo.h>

int main (int argc, char *argv[])
{

    int w, h;
    cairo_matrix_t   matrix;

    /* first we create a surface from the image that will be used as pattern */
    cairo_surface_t *s0 = cairo_image_surface_create_from_png ("batt0.png");
    w = cairo_image_surface_get_width (s0);
    h = cairo_image_surface_get_height (s0);
    printf("imsize= w:%i, h=%i\n", w, h); 
    cairo_pattern_t *pattern = cairo_pattern_create_for_surface (s0);
    // the pattern does not automatically tile over areas bigger than itself
    // in cairo lingo, the extend is NONE. et's change that to get the tile:
    // cairo_pattern_set_extend (pattern, CAIRO_EXTEND_REPEAT);
    cairo_pattern_set_extend (pattern, CAIRO_EXTEND_REFLECT);

    /* note how no cairo context will be needed for this */

    // cairo_t *cr0 = cairo_create (s0);
    // cairo_translate (cr, 128.0, 128.0);
    // // cairo_rotate (cr, M_PI / 4);
    // cairo_scale (cr, .5, .5);
    // // cairo_scale (cr, 1 / sqrt (2), 1 / sqrt (2));
    // cairo_translate (cr, -128.0, -128.0);

    cairo_surface_t *s2 = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 800, 600);
    // can change the size of the pattern here.
    // cairo_matrix_init_scale (&matrix, w/256.0 * 2.0, h/256.0 * 2.0);
    cairo_matrix_init_scale(&matrix, 2.0, 2.0);
    cairo_pattern_set_matrix (pattern, &matrix);

    cairo_t *cr = cairo_create (s2);
    cairo_set_source(cr, pattern); 
    /* we'll fil this rectangle with our pattern, note that the pattern will start
     * at a notional (0,0), so if our rectangle has an offset, the pattern will not start
     * there, but rather at (0,0) */
    // cairo_rectangle (cr, 30, 20, 700, 550);
    cairo_rectangle (cr, 0, 0, 700, 550);
    cairo_fill(cr);

    cairo_surface_write_to_png (s2, "pattap0.png");
    cairo_pattern_destroy (pattern);
    cairo_destroy (cr);
    cairo_surface_destroy (s0);
    cairo_surface_destroy (s2);

    return 0;
}
