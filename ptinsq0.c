#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cairo/cairo.h>

#define CVWD 640  // canvas wdth
#define CVHE 480 
#define HOFF 20
#define VOFF 30

#define NUMPTS 12

float *randf01(int howmany)
{
    int i;
    float *r=malloc(howmany*sizeof(float));
    for(i=0;i<howmany;++i) 
        r[i]= rand()/(float)RAND_MAX;
    return r;
}

int main (int argc, char *argv[])
{
    int i;
    int vext=CVHE-VOFF*2; // horiz extent
    int hext=CVWD-HOFF*2;

    /* cairo setup */
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, CVWD, CVHE);
    cairo_t *cr = cairo_create(surface);
    /*  first we're going to set the background canvas*/
    cairo_rectangle(cr, 0, 0, CVWD, CVHE);
    cairo_set_source_rgba(cr, 0, 0, 0, 0.95); /*  final number is alpha, 1.0 is opaque */
    cairo_fill(cr);
    cairo_rectangle (cr, HOFF, VOFF, hext, vext);
    cairo_set_source_rgb(cr, 0.5, 0.5, 0);
    cairo_stroke(cr);

    /* set vertical indices, will be the same each time if we set vpos correctly */
    float *pta=randf01(NUMPTS*2);

    /* dot placement */
    float x, y;
    cairo_set_source_rgba(cr, 0.85, 0.1, 0.2, 0.8);
    for(i=0;i<NUMPTS*2;i+=2) {
        x=hext*pta[i];
        y=vext*pta[i+1];
        cairo_arc(cr, x+HOFF, y+VOFF, 3, 0, 2 * M_PI);
        cairo_fill(cr);
    }

    cairo_surface_write_to_png (surface, "ptinsq0.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    free(pta);

    return 0;
}
