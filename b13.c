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
    int i, j, m, k, *vind, *rvind, *vpos, *totvpos, *tothpos, widthoffset, heightoffset;
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

    for(k=0;k<SQBLX;++k) {
        /*  first two vital assignments */
        vpos = totvpos +k*num_vpos;
        hpos = tothpos +k*VBARNUMS;
        heightoffset=SQHE*((k>>1) & 0x01); 
        widthoffset=SQWD*(((k>>1)^k)&0x01); /* verified */

        for(i=0;i<VBARNUMS;i++) {
            m=0;
            for(j=vind[i]; j<vind[i+1]; ++j) 
                vpos[j] = SQHE/(2<<i) + (m++) * SQHE/(1<<i) + heightoffset;
        }

        if(((k>>1)^k)&0x01) { /* that's the exclusive OR */

            for(i=0;i<VBARNUMS;i++)
                hpos[i]= hbar/2 + i*hbar + widthoffset;

        } else {
            /*  we go int he opposite direction */
            for(i=0;i<VBARNUMS;i++)
                hpos[i]= VBARNUMS*hbar - hbar/2 - i*hbar + widthoffset;
        }

        /* dot placement */
        cairo_set_source_rgba(cr, 0.85, 0.1, 0.2, 0.8);
        for(i=0;i<VBARNUMS;i++)
            for(j=vind[i]; j<vind[i+1]; ++j) {
                cairo_arc(cr, hpos[i], vpos[j], 3, 0, 2 * M_PI);
                cairo_fill(cr);
            }

        /* line placement */
        cairo_set_source_rgba(cr, 0.65, 0.8, 0.45, 1.0);
        cairo_set_line_width (cr, 0.75); /* thinnest really possible */
        for(i=1;i<VBARNUMS;i++)
            for(j=vind[i]; j<vind[i+1]; ++j) {
                cairo_move_to(cr,hpos[i],vpos[j]);
                cairo_line_to(cr,hpos[i-1],vpos[(j-1)>>1]);
                cairo_stroke(cr);
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

    /* a new array to integrate all the values */
    int narrwd=VBARNUMS*SQBLX + (1<<(2-1)); /* node seq 1,2 i.e first two levels, which equates to three entries vert, two entries hor*/
    int narrhe=num_vpos*SQBLX + (2<<1)-1; /* node seq 1,2 i.e first two levels, which equates to three entries vert, two entries hor*/
    int *nthpos, *ntvpos;/*  these are tot arrays, but I'm dropping the toti, the n stands for new */ 
    ntvpos=calloc(narrhe, sizeof(int));
    nthpos=calloc(narrwd, sizeof(int));

    ntvpos[0] = SQHE;
    ntvpos[1] =  3*SQHE/4;
    ntvpos[2] = 5*SQHE/4;
    /*  screw all that just get the array  */
    m=(2<<1)-1; /* three */
    for(i=0;i<VBARNUMS;i++)
        for(k=0;k<SQBLX;++k)
            for(j=vind[i]; j<vind[i+1]; ++j) 
                ntvpos[m++] = (totvpos+k*num_vpos)[j];

    nthpos[0] = SQWD;
    nthpos[1] = SQWD;
    m=(1<<(2-1)); /* two */
    for(i=0;i<VBARNUMS;i++)
        for(k=0;k<SQBLX;++k)
            nthpos[m++] = (tothpos+k*VBARNUMS)[i];

    printf("NTVPOS:\n"); 
    for(i=0;i<SQBLX;++i) {
        for(j=i*num_vpos;j<(i+1)*num_vpos;++j)
            printf("%03i ", ntvpos[j]); 
        printf("\n"); 
    }
    printf("\n"); 

    printf("NTHPOS:\n"); 
    for(i=0;i<SQBLX;++i) {
        for(j=0;j<VBARNUMS;j++)
            printf("%03i ", nthpos[i*VBARNUMS+ j]); 
        printf("\n"); 
    }
    printf("\n"); 

    cairo_surface_write_to_png (surface, "b13.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    free(ntvpos);
    free(nthpos);
    free(tothpos);
    free(totvpos);
    free(vind);

    return 0;
}
