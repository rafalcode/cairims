/* very worthwhile exercise, advancing step by step horizontally
 * and then by powers of two vertically. In this way a binomial pattern can be drawn with cairo
 * it's critical to see how those forloop indices are built up */
#include <cairo/cairo.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define HBARNUMS 5
#define LEAVES 3

typedef struct
{
    int h, v;
} pos;

int main (int argc, char *argv[])
{
    int i, j, width=480, height=480;
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create (surface);

    /*  first we're going to set the background */
    cairo_rectangle (cr, 0, 0, width, height); /* arg explan: topleftcorner and size of shape  */
    cairo_set_source_rgba (cr, 0, 0, 0, 0.95); /*  final number is alpha, 1.0 is opaque */
    cairo_fill (cr);

    /* vertical divider how ar ewe going to section off the screen vertically */
    float hbar=(float)width/HBARNUMS; /* answer is: equally */
    float vbar;
    pos ori, *lb=calloc(HBARNUMS, sizeof(pos)); /* ori: origin, lb, last bar, */
    /* arrange array of positions */
    ori.h=hbar/2.;
    ori.v= height /2.;
    cairo_set_source_rgba(cr, 0.65, 0.8, 0.45, 0.6);
    cairo_arc(cr, ori.h, ori.v, 2, 0, 2 * M_PI);
    cairo_fill(cr);

    float slope = (height/2. - height/LEAVES/2.) / (width - hbar); /* half height less top offset */

    for(j=HBARNUMS-1;j>0;--j) { 
        lb[0].h= width-hbar/2.-hbar*(HBARNUMS-1-j);
        lb[0].v = height/LEAVES/2. + slope * hbar*(HBARNUMS-1-j);
        // vbar=(float)height*(HBARNUMS-j)/pow(HBARNUMS,2);
        vbar=(height-lb[0].v*2) / (LEAVES-1);
        for(i=1;i<LEAVES;i++) {
            lb[i].h= lb[0].h;
            lb[i].v = lb[i-1].v + vbar;
        }

        if ((j%2) == 0)
            cairo_set_source_rgba(cr, 0.65, 0.8, 0.45, 0.6);
        else
            cairo_set_source_rgba(cr, 0.85, 0.1, 0.2, 0.8);

        for(i=0;i<LEAVES;i++){
            cairo_arc(cr, lb[i].h, lb[i].v, 1, 0, 2 * M_PI);
            cairo_fill(cr);
        }
    }

    /* Write output and clean up */
    cairo_surface_write_to_png (surface, "td2.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);
    free(lb);

    return 0;
}
