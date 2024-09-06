/* use trndot.c ibut to do to do a simple grid */
#include <cairo/cairo.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define HBARNUMS 32
#define VBARNUMS 32

typedef struct
{
    float x, y;
} pos_t;

int main (int argc, char *argv[])
{
    int i, j, width=640, height=480;
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create (surface);

    /*  first we're going to set the background */
    cairo_rectangle (cr, 0, 0, width, height); /* arg explan: topleftcorner and size of shape  */
    cairo_set_source_rgba (cr, 0, 0, 0, 0.95); /*  final number is alpha, 1.0 is opaque */
    cairo_fill (cr);

    /* vertical divider how ar ewe going to section off the screen vertically */
    float vbar=(float)width/VBARNUMS; // basis for x
    float hbar=(float)height/HBARNUMS; // basis for y
    int totpoints = HBARNUMS*VBARNUMS;
    pos_t *lb=calloc(totpoints, sizeof(pos_t)); /* ori: origin, lb, last bar, */

    // first point, top left
    lb[0].x= vbar/2.;
    lb[0].y = hbar/2.;
    // first column, left side downwards x is the same.
    for(j=1;j<HBARNUMS;j++) {
         lb[j].x= lb[j-1].x;
         lb[j].y= lb[j-1].y + hbar;
    }
    // first row top, y inmutable here
    for(i=1;i<VBARNUMS;i++) {
        lb[i*HBARNUMS].x= lb[(i-1)*HBARNUMS].x + vbar;
        lb[i*HBARNUMS].y= lb[(i-1)*HBARNUMS].y;
    }

    for(i=1;i<VBARNUMS;i++)
        for(j=1;j<HBARNUMS;j++) {
            lb[i*HBARNUMS+j].x= lb[i*HBARNUMS+j-1].x;
            lb[i*HBARNUMS+j].y= lb[i*HBARNUMS+j-1].y + hbar;
        }

    cairo_set_source_rgba(cr, 0.85, 0.2, 0.2, 1);
    for(i=0;i<totpoints;i++){
        // printf("%4.4f %4.4f\n", lb[i].x, lb[i].y);
        // cairo_arc(cr, lb[i].x, lb[i].y, 2, 0, 2 * M_PI);
        // cairo_fill(cr);

        // cairo_move_to(cr,ori.h,ori.v);
        // cairo_line_to(cr,lb[i].h, lb[i].v);
        cairo_stroke(cr);
    }

    /* Write output and clean up */
    cairo_surface_write_to_png (surface, "gri0.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);
    free(lb);

    return 0;
}
