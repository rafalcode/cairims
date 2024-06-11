/* just generating an index file from vbars[] */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cairo/cairo.h>

#define VBARNUMS 4
#define SQBLX 4 // aka. number of quandrants ... a bit silly, will always be 4 pretty much
// following is size of each quadrant in 2D
#define SQWD 320 
#define SQHE 240 

const float TFONTSZ=16.0; /* Title font size */

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
    // vind[0]=0; //calloc took care of that.
    for(i=1;i<=VBARNUMS;i++)
        vind[i] = (1<<i)-1;
    /* just to check */
    printf("Checking vind[]:\n"); 
    for(i=0;i<=VBARNUMS;i++)
        printf("%i ", vind[i]);
    printf("\n"); 
    printf("Note how difference between vind elements is the canonical binary sequence.\n"); 

    int *hmid=malloc(SQBLX*sizeof(int));
    int *vmid=malloc(SQBLX*sizeof(int));

    for(k=0;k<SQBLX;++k) {
        /*  first two vital assignments */
        vpos = totvpos +k*num_vpos;
        hpos = tothpos +k*VBARNUMS;
        heightoffset=SQHE*((k>>1) & 0x01); //decides upper or lower pair of quadrants
        widthoffset=SQWD*(((k>>1)^k)&0x01); /* verified: left or right pair of quadrants? */

        // note only the horizontal move need be concerned about which quadrant we are in
        // the vertical positions don't care, except for upper and lower, 
        // in which case there's just a differnet offset
        for(i=0;i<VBARNUMS;i++) {
            m=0;
            for(j=vind[i]; j<vind[i+1]; ++j) 
                vpos[j] = SQHE/(2<<i) + (m++) * SQHE/(1<<i) + heightoffset;
        }

        // the hpos order is from root node out, so in the right pair of quadrants,
        // it follows left to right, but on the left it's right to left ... arabic-scriptwise
        if(((k>>1)^k)&0x01) { /* that's the exclusive OR */
            for(i=0;i<VBARNUMS;i++)
                hpos[i]= hbar/2 + i*hbar + widthoffset;
            // no minus', so that's the right pair of quadrants

        } else {
            /*  arabic-scriptwise, right to left. */
            for(i=0;i<VBARNUMS;i++)
                hpos[i]= VBARNUMS*hbar - hbar/2 - i*hbar + widthoffset;
        }

        /* dot placement */
        cairo_set_source_rgba(cr, 0.85, 0.1, 0.2, 0.8);
        for(i=0;i<VBARNUMS;i++) {
            for(j=vind[i]; j<vind[i+1]; ++j) {
                cairo_arc(cr, hpos[i], vpos[j], 3, 0, 2 * M_PI);
                cairo_fill(cr);
            }
            if(i==0) {
                printf("i=0: %i|%i\n",hpos[vind[i]], vpos[vind[i]]);
                hmid[k]=hpos[vind[i]];
                vmid[k]=vpos[vind[i]];
            }
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

    //dots
    cairo_set_source_rgba(cr, 0.85, 0.1, 0.2, 0.8);
    cairo_arc(cr, hmid[0] +(hmid[1]-hmid[0])/2, vmid[0]+(vmid[2]-vmid[0])/4, 3, 0, 2 * M_PI);
    cairo_fill(cr);
    // cairo_arc(cr, hmid[0] +(hmid[1]-hmid[0])/2, vmid[0]+(vmid[2]-vmid[0])/2, 3, 0, 2 * M_PI);
    // cairo_fill(cr);
    cairo_arc(cr, hmid[2] +(hmid[3]-hmid[2])/2, vmid[2]-(vmid[2]-vmid[0])/4, 3, 0, 2 * M_PI);
    cairo_fill(cr);

    // do the central bit
    cairo_set_source_rgba(cr, 0.65, 0.8, 0.45, 1.0);
    cairo_move_to(cr, hmid[0],vmid[0]);
    cairo_line_to(cr, hmid[0] +(hmid[1]-hmid[0])/2, vmid[0]+(vmid[2]-vmid[0])/4);
    cairo_line_to(cr, hmid[0] +(hmid[1]-hmid[0])/2, vmid[0]-TFONTSZ+(vmid[2]-vmid[0])/2);
    cairo_stroke(cr);
    cairo_move_to(cr, hmid[1],vmid[1]);
    cairo_line_to(cr, hmid[1] -(hmid[1]-hmid[0])/2, vmid[1]+(vmid[3]-vmid[1])/4);
    cairo_stroke(cr);

    cairo_move_to(cr, hmid[2],vmid[2]);
    cairo_line_to(cr, hmid[2] +(hmid[3]-hmid[2])/2, vmid[2]-(vmid[2]-vmid[0])/4);
    cairo_line_to(cr, hmid[2] +(hmid[3]-hmid[2])/2, vmid[2]+TFONTSZ -(vmid[2]-vmid[0])/2);
    cairo_stroke(cr);
    cairo_move_to(cr, hmid[3],vmid[3]);
    cairo_line_to(cr, hmid[3] -(hmid[3]-hmid[2])/2, vmid[3]-(vmid[3]-vmid[1])/4);
    cairo_stroke(cr);

    cairo_select_font_face (cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_font_extents_t fe;
    cairo_text_extents_t te;
    cairo_set_font_size (cr, TFONTSZ);
    char tistr[]= "DNA";
    cairo_text_extents (cr, tistr, &te);
    cairo_font_extents (cr, &fe);

    cairo_move_to (cr, SQWD - te.x_bearing - te.width/2, SQHE - fe.descent + fe.height/2);
    // cairo_move_to (cr, SQWD - te.x_bearing - te.width/2, SQHE - fe.descent);
    cairo_set_source_rgb(cr, 0.5, 0.6, .782);
    cairo_show_text (cr, tistr);



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
    free(vmid);
    free(hmid);
    free(vind);

    return 0;
}
