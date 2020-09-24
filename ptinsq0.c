#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cairo/cairo.h>

#define CVWD 640  // canvas wdth
#define CVHE 480 
#define LHOFF 50 // left hor
#define UVOFF 50 // upper vert
#define RHOFF 20
#define LVOFF 50

#define NUMPTS 20

float *randf01(int howmany) // uniform random numbers [0,1]
{
    int i;
    float *r=malloc(howmany*sizeof(float));
    for(i=0;i<howmany;++i) 
        r[i]= rand()/(float)RAND_MAX;
    return r;
}

float *pwdf(float *pta, int howmany)
{
    int i, j, ii, jj, ra=0;
    pwdsz=(howmany-1)*howmany/2;

    float *pwda=malloc(pwdsz*sizeof(float));
    for(i=1;i<howmany;++i) {
        ii=i*2;
        for(j=i;j<howmany;++j) 
            jj=j*2;
            pwda[j+ra]= sqrt(pow(pta[ii] - pta[ii-2], 2)  + pow(pta[i+1] - pta[i-1], 2)
        ra += howmany-i;
    }
    return r;
}

int main (int argc, char *argv[])
{
    int i;
    int vext=CVHE-UVOFF-LVOFF; // vert extent
    int hext=CVWD-LHOFF-RHOFF; // horiz extent

    /* cairo setup */
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, CVWD, CVHE);
    cairo_t *cr = cairo_create(surface);
    /*  first we're going to set the background canvas*/
    cairo_rectangle(cr, 0, 0, CVWD, CVHE);
    cairo_set_source_rgba(cr, 0, 0, 0, 0.95); /*  final number is alpha, 1.0 is opaque */
    cairo_fill(cr);

    /* setting up the barest of axes. NOTE: line coordinates must be absolute ... not extents as in rectangle() */
    cairo_move_to(cr, LHOFF-1.5, UVOFF-1.5);
    cairo_line_to(cr, LHOFF-1.5, UVOFF-1.5+vext+3);
    cairo_move_to(cr, LHOFF-1.5, UVOFF-1.5+vext+3);
    cairo_line_to(cr, LHOFF-1.5+hext+3, UVOFF-1.5+vext+3);
    cairo_move_to(cr, LHOFF-1.5+hext+3, UVOFF-1.5+vext+3);
    cairo_line_to(cr, LHOFF-1.5+hext+3, UVOFF-1.5+vext+3+6); //tick
    cairo_set_source_rgba(cr, 0.65, 0.8, 0.45, 1.0);
    cairo_set_line_width (cr, 0.75); /* thinnest really possible */
    cairo_stroke(cr);

    /* set vertical indices, will be the same each time if we set vpos correctly */
    float *pta=randf01(NUMPTS*2); // taken in pairs, can be viewed as x,y coords.

    /* dot placement */
    float x, y;
    cairo_set_source_rgba(cr, 0.85, 0.1, 0.2, 0.8);
    for(i=0;i<NUMPTS*2;i+=2) {
        x=hext*pta[i];
        y=vext*(1.-pta[i+1]); // cairo goes down the axis, so we must reverse
        cairo_arc(cr, x+LHOFF, y+UVOFF, 2, 0, 2 * M_PI);
        cairo_fill(cr);
    }

    cairo_surface_write_to_png (surface, "ptinsq0.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    free(pta);

    return 0;
}
