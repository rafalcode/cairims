// gradients!
// from https://zetcode.com/gfx/cairo/gradients/
// cairo patterns are not that esy to get
#include <cairo/cairo.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    int i, k, width=320, height=320;
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create (surface);

    // the following sets the extent of the repeating pattern
    // in a way it's like the overall loop, but not th elements of the loop itself.
    cairo_pattern_t *pat1 = cairo_pattern_create_linear(0.0, 0.0, width, height);

    // the following sets the pattern that will be repeated.
    // you need two cairo_pattern_add_color_stop_rgb() calls for this.
    double joffset; // this is a special pattern variable: the "offset". It appears it should go from zero to 1.0
    double jsteps=.05; // this is a special pattern variable: the "offset". It appears it should go from zero to 1.0. Yes. refman confirms.
    int count = 1;
    for ( joffset = 0; joffset < 1.; joffset += jsteps) {
        if(count % 2) {
            // cairo_pattern_add_color_stop_rgb(pat1, j, 0, 0, 0);
            cairo_pattern_add_color_stop_rgb(pat1, joffset, 0.15, .1, .1);
            printf("Set %i (Uneven) @ joffset=%.2f first: %.2f %.2f %.2f\n", count, joffset, 0.15, .1, .1);
        } else { 
            // cairo_pattern_add_color_stop_rgb(pat1, j, 1, 0, 0);
            cairo_pattern_add_color_stop_rgb(pat1, joffset, .5, .1, .1);
            printf("Set %i (Even) @ joffset=%.2f first: %.2f %.2f %.2f\n", count, joffset, 0.5, .1, .1);
        }
        count++;
    }

    // cairo_rectangle(cr, 20, 20, 300, 100);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_set_source(cr, pat1);
    cairo_fill(cr);  

    cairo_pattern_destroy(pat1);
    cairo_surface_write_to_png (surface, "gra0.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);
    return 0;
}
