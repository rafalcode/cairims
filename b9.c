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

int *crearvarr(int *arr, int sz)
{
    int i, j=0, *rvarr;
    rvarr=calloc(sz, sizeof(int));
    for(i=sz-1;i>=0;--i)
        rvarr[j++] = arr[i];
    return rvarr;
}

int main (int argc, char *argv[])
{
    int canvw= SQWD*SQBLX/2, canvh=SQHE*SQBLX/2;
    int i, j, m, k, *vind, *rvind, *vpos, *rvpos, *totvpos, *tothpos, widthoffset, heightoffset;
    int *hpos, hbar=SQWD/VBARNUMS;
    /* vertical divider how ar ewe going to section off the screen vertically */

    int num_vpos=(1<<VBARNUMS)-1;
    totvpos=calloc(num_vpos*SQBLX, sizeof(int));
    tothpos=calloc(VBARNUMS*SQBLX, sizeof(int));
    /* cairo setup */
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, canvw, canvh);
    cairo_t *cr = cairo_create (surface);
    /*  first we're going to set the background */
    cairo_rectangle (cr, 0, 0, canvw, canvh); /* arg explan: topleftcorner and size of shape  */
    cairo_set_source_rgba (cr, 0, 0, 0, 0.95); /*  final number is alpha, 1.0 is opaque */
    cairo_fill (cr);

    /* set vertical indices, will be the same each time if we set vpos correctly */
    vind=calloc(VBARNUMS+1, sizeof(int));
    vind[0]=0;
    for(i=0;i<VBARNUMS;i++)
        vind[i+1] = (1<<(i+1))-1;
    /* just to check */
    printf("Checking vind[]:\n"); 
    for(i=0;i<=VBARNUMS;i++)
        printf("%i ", vind[i]);
    printf("\n"); 

    printf("\nNow reverse vind[]:\n"); 
    rvind=crearvarr(vind, VBARNUMS+1);
    for(i=0;i<=VBARNUMS;i++)
        printf("%i ", rvind[i]);
    printf("\n"); 

    cairo_set_source_rgba(cr, 0.65, 0.8, 0.45, 0.6);
    for(k=0;k<SQBLX;++k) {
        /*  first two vital assignments */
        vpos = totvpos +k*num_vpos;
        hpos = tothpos +k*VBARNUMS;
        heightoffset=SQHE*((k>>1)^(k & 0x01)); // is same as (k%2 +k/2)%2
        widthoffset=SQWD*((k>>1)==0); /* verified */

        for(i=0;i<VBARNUMS;i++) {
            m=0;
            for(j=vind[i]; j<vind[i+1]; ++j) 
                vpos[j] = SQHE/(2<<i) + (m++) * SQHE/(1<<i) + heightoffset;
        }

        if((k/2)==0) {

            for(i=0;i<VBARNUMS;i++)
                hpos[i]= hbar/2 + i*hbar + widthoffset;

        } else {

            for(i=0;i<VBARNUMS;i++)
                hpos[i]= VBARNUMS*hbar - hbar/2 - i*hbar + widthoffset;
        }

        for(i=0;i<VBARNUMS;i++)
            for(j=vind[i]; j<vind[i+1]; ++j) {
                cairo_arc(cr, hpos[i], vpos[j], 1, 0, 2 * M_PI);
                cairo_fill(cr);
            }

    }

    printf("TOTVPOS:\n"); 
    for(i=0;i<SQBLX;++i) {
        for(j=i*num_vpos;j<(i+1)*num_vpos;++j)
            printf("%03i ", totvpos[j]); 
        printf("\n"); 
    }
    printf("\n"); 

    printf("TOTHPOS:\n"); 
    for(i=0;i<SQBLX;++i) {
        for(j=0;j<VBARNUMS;j++)
            printf("%03i ", tothpos[i*VBARNUMS+ j]); 
        printf("\n"); 
    }
    printf("\n"); 

    cairo_surface_write_to_png (surface, "b9.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    free(totvpos);
    free(tothpos);
    free(vind);
    free(rvind);

    return 0;
}
