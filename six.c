/* just generating an index file from vbars[] */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cairo/cairo.h>

#define VBARNUMS 4
#define SQBLX 4
#define SQWD 320 
#define SQHE 240 

int main (int argc, char *argv[])
{
    int canvw= SQWD*SQBLX/2, canvh=SQHE*SQBLX/2;
    int i, j, m, k, *vind, *rvind, widthoffset, heightoffset;
    int hbar=SQWD/VBARNUMS;
    /* vertical divider how ar ewe going to section off the screen vertically */

    int *vp, *hp;
    hp=calloc(7,sizeof(int));
    vp=calloc(7,sizeof(int));
    hp[0] = hp[1]=hp[2] = SQWD;
    vp[0] = SQHE;
    vp[1] = SQHE - SQHE/4;
    vp[2] = SQHE + SQHE/4;

    /* cairo setup */
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, canvw, canvh);
    cairo_t *cr = cairo_create (surface);
    /*  first we're going to set the background */
    cairo_rectangle (cr, 0, 0, canvw, canvh); /* arg explan: topleftcorner and size of shape  */
    cairo_set_source_rgba (cr, 0, 0, 0, 0.95); /*  final number is alpha, 1.0 is opaque */
    cairo_fill (cr);

    /* set vertical indices, will be the same each time if we set vpos correctly */
    vind=calloc(3+1, sizeof(int));
    vind[0]=0;
    for(i=0;i<3;i++) /* thought of eight, but then should be 3, as in power of 2 */
        vind[i+1] = (1<<(i+1))-1; /*  should give 1, 2 and 4 */
    /* just to check */
    printf("Checking vind[]:\n"); 
    for(i=0;i<=3;i++)
        printf("%i ", vind[i]);
    printf("\n"); 

    for(k=0;k<SQBLX;++k) {
        /*  first two vital assignments */
        heightoffset=SQHE*((k>>1) & 0x01);

        // hp[k+3]= SQWD + hbar/2 - hbar * ((((k>>1) & 0x01)^(k & 0x01))==0); /*  that¡s the anti-exclusive OR on the last two bits */
        hp[k+3]= SQWD + hbar/2 - hbar * ((((k>>1)^k) & 0x01)==0); /*  that¡s the anti-exclusive OR on the last two bits */
        vp[k+3]= SQHE/2 + heightoffset;
    }

    /* dot placement */
    cairo_set_source_rgba(cr, 0.85, 0.1, 0.2, 0.8);
    for(i=0;i<4;i++)
        for(j=vind[i]; j<vind[i+1]; ++j) {
            cairo_arc(cr, hp[j], vp[j], 3, 0, 2 * M_PI);
            cairo_fill(cr);
        }

    /* line placement */
    cairo_set_source_rgba(cr, 0.65, 0.8, 0.45, 1.0);
    cairo_set_line_width (cr, 0.75); /* thinnest really possible */
    for(i=1;i<3;i++)
        for(j=vind[i]; j<vind[i+1]; ++j) {
            cairo_move_to(cr,hp[j],vp[j]);
            cairo_line_to(cr,hp[(j-1)>>1],vp[(j-1)>>1]);
            cairo_stroke(cr);
        }

    printf("HP:\n"); 
    for(i=0;i<7;++i) {
        printf("%03i ", hp[i]); 
    }
    printf("\n"); 
    printf("VP:\n"); 
    for(i=0;i<7;++i) {
        printf("%03i ", vp[i]); 
    }
    printf("\n"); 

    cairo_surface_write_to_png (surface, "six.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    free(hp);
    free(vp);
    free(vind);

    return 0;
}
