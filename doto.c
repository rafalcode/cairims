/* very worthwhile exercise, advancing step by step horizontally
 * and then by powers of two vertically. In this way a binomial pattern can be drawn with cairo
 * it's critical to see how those forloop indices are built up */
#include <cairo/cairo.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    int width=64, height=64;
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create (surface);

    /*  first we're going to set the background */
    cairo_rectangle (cr, 0, 0, width, height); /* arg explan: topleftcorner and size of shape  */
    cairo_set_source_rgb(cr, 0, 0, 0); /*  final number is alpha, 1.0 is opaque */
    cairo_fill (cr);

    /* Drawing code goes here */
    cairo_set_source_rgb(cr, 0.65, 0.8, 0.45);
    cairo_arc(cr, width/2, height/2, 8, 0, 2 * M_PI);
    cairo_fill(cr);
    /* Write output and clean up */
    cairo_surface_write_to_png (surface, "doto.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    return 0;
}
