/* a more advanced version of image block example */
#include <cairo/cairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "colorhdr.h" // look for colorist

/* now the principle canvas definition settings, which is the image dimensions minus the margins
 * follows vim direction conventions*/
#define IMLMARG 20 /* left margin */
#define IMBMARG 10 /* bottom */
#define IMRMARG 30
#define IMTMARG 40

/* principal image and block settings */
#define IMWIDTH 640
#define IMHEIGHT 640
#define CBNUM 4 /* number of column bands */
#define RBNUM CBNUM /* number of row-bands */

/* derived settings */
#define CBSZ IMWIDTH/CBNUM /* size of each column-band */
#define RBSZ IMHEIGHT/RBNUM /* size of each row-band */

/* Margins of each block */
#define BKLMARG 0 /* left margin */
#define BKBMARG 0 /* bottom */
#define BKRMARG 0
#define BKTMARG 0

#define CS_SZ 18 /* the size (number of colours) that our colorscheme has */
const float FONTSZ=12.0; /* the size (number of colours) that our colorscheme has */

int main (int argc, char *argv[])
{

    float xtmodx=(float)IMLMARG/CBNUM; /* extension mod in x-dir */
    float xtmody=(float)IMTMARG/RBNUM; /* extension mod in x-dir */

    /* SPECIAL NOTE: we use the header file which already has a global colour sturc and array(called "colorist"), please see colorhdr.h */
    colstrucfloat colsf;
    char *colsfnam=NULL;

    cairo_surface_t *surface;
    cairo_t *cr;

    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, IMWIDTH, IMHEIGHT);
    cr = cairo_create (surface);
    /* First we set our general background */
    cairo_rectangle (cr, 0, 0, IMWIDTH, IMHEIGHT); /* arg explan: topleftcorner and size of shape  */
    cairo_set_source_rgba (cr, 0, 0, 0, 0.95); /*  final number is alpha, 1.0 is opaque */
    cairo_fill (cr);

    /* OK, now, text consideration */
    cairo_select_font_face (cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size (cr, FONTSZ);
    cairo_font_extents_t fe;
    cairo_text_extents_t te;

    int i, j, k=0;
    float rstx /*rect start x-dir */, rsty /*rect start y-dir */, rxtx /*rect extent x-dir */, rxty /*rect start y-dir */;
    for (j=0; j<CBNUM; j++) { /* i and j serve as our block indices */
        colsfnam = colorist[(k+2)/4].gnam;
        cairo_text_extents (cr, colsfnam, &te);
        cairo_move_to (cr, (CBSZ*j+CBSZ/2) - te.x_bearing - te.width/2, IMTMARG/2 - fe.descent + fe.height/2);
        cairo_set_source_rgb(cr, 0.9, 0.9, .9);
        cairo_show_text (cr, colsfnam);
    }
    for (i=0; i<RBNUM; i++)
        for (j=0; j<CBNUM; j++) { /* i and j serve as our block indices */
            k=CBNUM*i+j+2;
            colsf = colorist[k];

            cairo_set_source_rgb(cr, colsf.rgbflo[0], colsf.rgbflo[1], colsf.rgbflo[2]);
            rstx=IMLMARG + CBSZ*j+BKLMARG;
            rsty=IMTMARG + RBSZ*i+BKTMARG;
            rxtx=CBSZ-BKLMARG-BKRMARG-xtmodx;
            rxty=RBSZ-BKTMARG-BKBMARG-xtmody;
            cairo_rectangle (cr, rstx, rsty, rxtx, rxty);
            cairo_fill (cr);
        }

    /* Write output and clean up */
    cairo_surface_write_to_png (surface, "rectcolcat.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    /* Finally some info on the color array */
    printf("Colorist color array has %zu colours\n", sizeof(colorist) / sizeof(colstrucfloat));

    return 0;
}
