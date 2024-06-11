/* use trndot.c ibut to do to do a simple grid */
#include <cairo/cairo.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "wavtab.h"

#define HBARNUMS 3
#define NXPTS 256

int main (int argc, char *argv[])
{

    int i, j, width=640, height=480;
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create (surface);

    /*  first we're going to set the background */
    cairo_rectangle (cr, 0, 0, width, height); /* arg explan: topleftcorner and size of shape  */
    cairo_set_source_rgb(cr, 0, 0, 0); 
    cairo_fill (cr);

    /* vertical divider how ar ewe going to section off the screen vertically */
    double vbar=(double)width/NXPTS; // basis for x
    double *xpts=calloc(NXPTS, sizeof(double)); /* ori: origin, pt, last bar, */
    double *ypts=calloc(NXPTS, sizeof(double)); /* ori: origin, pt, last bar, */

    xpts[0]=vbar/2;
    for(i=1;i<NXPTS;i++)
        xpts[i] = xpts[i-1] +vbar;
    double midv=height/2;
    double span=midv-5;
    double vinc= midv/NXPTS;
    // ypts[0]=midv;
    for(i=0;i<NXPTS;i++) {
        // ypts[i]=ypts[i-1]-vinc;
        ypts[i]=midv - span*wtvals[i]/0x7FFF;
        printf("%2.4f ", ypts[i]);
    }
    printf("\n"); 

    double upperv=height/4;
    cairo_set_source_rgba(cr, 0.65, 0.8, 0.45, .85);
    cairo_set_line_width (cr, 0.25); /* thinnest really possible */
    for(i=0;i<NXPTS;i++){
        if( (ypts[i]> (midv-2)) & (ypts[i] < (midv+2)) ) {
            cairo_arc(cr, xpts[i], midv, .8, 0, 2 * M_PI);
            cairo_fill(cr);
        } else {
            cairo_move_to(cr, xpts[i], midv);
            cairo_line_to(cr, xpts[i], ypts[i]);
        }

        // cairo_move_to(cr,ori.h,ori.v);
        // cairo_line_to(cr,pt[i].h, pt[i].v);
        cairo_stroke(cr);
    }

    /* Write output and clean up */
    cairo_surface_write_to_png (surface, "gri1.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);
    free(xpts);
    free(ypts);

    return 0;
}
