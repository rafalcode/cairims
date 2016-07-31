/* very worthwhile exercise, advancing step by step horizontally
 * and then by powers of two vertically. In this way a binomial pattern can be drawn with cairo
 * it's critical to see how those forloop indices are built up */
#include <cairo/cairo.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum
{
    CENCIRC /* center circle */,
    CENRECT /* center retangle */
} sha_t;

int main (int argc, char *argv[])
{
   /* argument accounting: here were present the siddferetn shapes that are possible with this program */
	if(argc!=2) {
		printf("Error. Pls supply argument: shape type name, one of the following:\n");
		printf("1) \"cencirc\" for centered circle 2) \"cenrect\" for centred rectangle\n");
		exit(EXIT_FAILURE);
	}

    sha_t ourtype;
    if(!strcmp("cencirc", argv[1])) {
        ourtype=CENCIRC;
    } else if(!strcmp("cenrect", argv[1])) {
        ourtype=CENRECT;
    } else {
		printf("Error. Unrecognised shape type\n");
		exit(EXIT_FAILURE);
    }

    int width=64, height=64;
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create (surface);

    /*  first we're going to set the background */
    cairo_rectangle (cr, 0, 0, width, height); /* arg explan: topleftcorner and size of shape  */
    cairo_set_source_rgb(cr, 0, 0, 0); /*  final number is alpha, 1.0 is opaque */
    cairo_fill (cr);

    /* Drawing code goes here */
    cairo_set_source_rgb(cr, 1, 0.5, 0.5);
    switch(ourtype) {
        case CENCIRC:
            cairo_arc(cr, width/2, height/2, 8, 0, 2 * M_PI); break;
        case CENRECT:
            cairo_rectangle (cr, width/4, height/4, width/2, height/2); /* simple rectangle */
            cairo_rotate (cr, 25* M_PI/180); /* rotate it */ break;
    }
    cairo_fill(cr);
    /* Write output and clean up */
    cairo_surface_write_to_png (surface, "sma0.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    return 0;
}
