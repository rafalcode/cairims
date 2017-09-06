/* radiate image ... lines radiating from a point, but SVG style */
#include <cairo/cairo.h>
#include <cairo/cairo-svg.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define HBARNUMS 8

typedef struct
{
    double h, v;
} pos;

int main (int argc, char *argv[])
{
	if(argc!=2) {
		printf("Error. Pls supply argument: name of SVG file WITH extension.\n");
		exit(EXIT_FAILURE);
	}

    int i;
   	double width=480, height=480;
    cairo_surface_t *srf = cairo_svg_surface_create(argv[1], width, height);
	cairo_svg_surface_restrict_to_version (srf, CAIRO_SVG_VERSION_1_2);
    cairo_t *cr = cairo_create(srf);

    /*  i usually set background here ... but for SVG won't bother */

    /* vertical divider how ar ewe going to section off the screen vertically */
    double hbar=(double)width/HBARNUMS; /* answer is: equally */
    double vbar=(double)height/HBARNUMS; /* answer is: equally */
    pos ori, *lb=calloc(HBARNUMS, sizeof(pos)); /* ori: origin, lb, last bar, */
    /* arrange array of positions */
    ori.h=hbar/2.;
    ori.v=height/2.;

    lb[0].h= width-hbar/2.;
    lb[0].v = vbar/2.;
    for(i=1;i<HBARNUMS;i++) {
        lb[i].h= width-hbar/2.;
        lb[i].v = lb[i-1].v + vbar;
    }

    cairo_set_source_rgba(cr, 0., 0., 1., 1.);
    for(i=0;i<HBARNUMS;i++){
        // cairo_arc(cr, lb[i].h, lb[i].v, 1, 0, 2 * M_PI);
        // cairo_fill(cr);

        cairo_move_to(cr,ori.h,ori.v);
        cairo_line_to(cr,lb[i].h, lb[i].v);
        cairo_stroke(cr);
    }

    /* Write output and clean up: for SVG output file has already been defined. */
    cairo_destroy (cr);
    cairo_surface_destroy(srf);
    free(lb);

    return 0;
}
