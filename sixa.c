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

/* only global */
const float TFONTSZ=12.0; /* Title font size */

int main (int argc, char *argv[])
{
    int canvw= SQWD*SQBLX/2, canvh=SQHE*SQBLX/2;
    int i, j, m, k, *vind, *rvind, widthoffset, heightoffset;
    int hbar=SQWD/VBARNUMS;
    /* vertical divider how ar ewe going to section off the screen vertically */

    /* cairo setup */
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, canvw, canvh);
    cairo_t *cr = cairo_create (surface);
    /*  first we're going to set the background */
    cairo_rectangle (cr, 0, 0, canvw, canvh); /* arg explan: topleftcorner and size of shape  */
    cairo_set_source_rgba (cr, 0, 0, 0, 0.95); /*  final number is alpha, 1.0 is opaque */
    cairo_fill (cr);

    // trhe manually set pints
    int nummanpts =4;
    int *hp=calloc(nummanpts+4,sizeof(int));
    int *vp=calloc(nummanpts+4,sizeof(int));
    hp[0] = hp[1]=hp[2]= hp[3]= SQWD;
    vp[0] = SQHE-6;
    vp[1] = SQHE+6;
    vp[2] = SQHE - SQHE/4;
    vp[3] = SQHE + SQHE/4;

    cairo_select_font_face (cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_font_extents_t fe;
    cairo_text_extents_t te;
    cairo_set_font_size (cr, TFONTSZ);
    char tistr[]= "DNA";
    cairo_text_extents (cr, tistr, &te);

    cairo_move_to (cr, SQWD - te.x_bearing - te.width/2, SQHE - fe.descent + fe.height/2);
    cairo_set_source_rgb(cr, 0.5, 0.6, .782);
    cairo_show_text (cr, tistr);

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
        hp[k+nummanpts]= SQWD + hbar/2 - hbar * ((((k>>1)^k) & 0x01)==0); /*  that¡s the anti-exclusive OR on the last two bits */
        vp[k+nummanpts]= SQHE/2 + heightoffset;
    }

    /* dot placement:
     * starts at centre with 1, and fans out  */
    cairo_set_source_rgba(cr, 0.85, 0.1, 0.2, 0.8);
    for(i=1;i<4;i++) {
        for(j=vind[i]+1; j<vind[i+1]+1; ++j) {
            cairo_arc(cr, hp[j], vp[j], 3, 0, 2 * M_PI);
            cairo_fill(cr);
            printf("dot at %i,%i\n", hp[j], vp[j]);
        }
        printf("\n"); 
    }

    /* line placement */
    cairo_set_source_rgba(cr, 0.65, 0.8, 0.45, 1.0);
    cairo_set_line_width (cr, 0.75); /* thinnest really possible */
    for(i=2;i<3;i++)
        for(j=vind[i]+1; j<vind[i+1]+1; ++j) {
            cairo_move_to(cr,hp[j],vp[j]);
            cairo_line_to(cr,hp[(j-1)>>1],vp[(j-1)>>1]);
            cairo_stroke(cr);
        }

    printf("HP:\n"); 
    for(i=0;i<8;++i) {
        printf("%03i ", hp[i]); 
    }
    printf("\n"); 
    printf("VP:\n"); 
    for(i=0;i<8;++i) {
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
