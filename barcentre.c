/* just generating an index file from vbars[] */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cairo/cairo.h>

#define NUMHL 7 // number of horizontal lines in centre zone (for all image) .. must be odd!
#define NUMVL 15 // number of horizontal lines in centre zone (for all image)
#define WID 640 
#define HEI 480 

int main (int argc, char *argv[])
{
    int i, j;

    // hor bars.
    float *vcoords=malloc((NUMHL-1)*sizeof(float));
    int vcentre = HEI/2;
    float vdist = HEI/NUMHL; //yesp, not -1 .. so theere vdist/2 speace left on top and on bottom of image.
    // yes, NUMHL-1 because the centre line is special and has its own variable.
    for(i=0;i<NUMHL-1;i+=2) {
        j=1+i/2;
        vcoords[i] = vcentre - j*vdist;
        vcoords[i+1] = vcentre + j*vdist;
    }

    // ver bars.
    float *hcoords=malloc((NUMVL-1)*sizeof(float));
    int hcentre = WID/2;
    float hdist = WID/NUMVL;
    for(i=0;i<NUMVL-1;i+=2) {
        j=i/2;
        hcoords[i] = hcentre - j*hdist;
        hcoords[i+1] = hcentre + j*hdist;
    }
    printf("\n"); 

    for(i=0;i<NUMHL-1;++i) 
        printf("%2.2f ", vcoords[i]); 
    printf("\n"); 
    for(i=0;i<NUMVL-1;++i) 
        printf("%2.2f ", hcoords[i]); 
    printf("\n"); 

    /* cairo setup */
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, WID, HEI);
    cairo_t *cr = cairo_create (surface);
    /*  first we're going to set the background */
    cairo_rectangle (cr, 0, 0, WID, HEI); /* arg explan: topleftcorner and size of shape  */
    cairo_set_source_rgba (cr, 0, 0, 0, 0.95); /*  final number is alpha, 1.0 is opaque */
    cairo_fill (cr);

    // lines: h
    cairo_set_source_rgba(cr, 0.65, 0.8, 0.45, 1.0);
    cairo_set_line_width (cr, 0.75); /* thinnest really possible */
    cairo_move_to(cr, 0, vcentre);
    cairo_line_to(cr, WID, vcentre);
    for(i=0;i<NUMHL-1;++i) {
        cairo_move_to(cr, 0, vcoords[i]);
        cairo_line_to(cr, WID, vcoords[i]);
        cairo_stroke(cr);
    }
    // lines: v
    cairo_set_source_rgba(cr, 0.45, 0.8, 0.65, 1.0);
    cairo_set_line_width (cr, 0.75); /* thinnest really possible */
    // cairo_move_to(cr, hcentre, 0);
    // cairo_line_to(cr, hcentre, HEI);
    for(i=0;i<NUMVL-1;++i) {
        cairo_move_to(cr, hcoords[i], 0);
        cairo_line_to(cr, hcoords[i], HEI);
        cairo_stroke(cr);
    }
    // go back to this vind idea, vertical indices
    // actually better word outer limits.
    int extent = NUMHL/2; // integer div, if NUMHL=7, we get 3.
    int *vind=calloc(extent+1, sizeof(int));
    for(i=1;i<=extent;++i) 
        vind[i] = 0x01<<i;

    // dots
    cairo_set_source_rgba(cr, 0.85, 0.1, 0.2, 0.8);
    cairo_arc(cr, hcentre, vcentre, 3, 0, 2 * M_PI);
    cairo_fill(cr);
    for(i=0;i<2*extent;i+=2) {
        for(j=vind[i];j<vind[i+1];++j)  {
            // printf("dot:%2.2f|%2.2f ", hcoords[j], vcoords[i]);
            cairo_arc(cr, hcoords[j], vcoords[i], 6, 0, 2 * M_PI);
            cairo_fill(cr);
            // printf("dot:%2.2f|%2.2f ", hcoords[j], vcoords[i+1]);
            cairo_arc(cr, hcoords[j], vcoords[i+1], 6, 0, 2 * M_PI);
            cairo_fill(cr);
        }
        printf("\n"); 
    }

    cairo_surface_write_to_png (surface, "barcentre.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    free(vind);
    free(hcoords);
    free(vcoords);

    return 0;
}
