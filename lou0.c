#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cairo/cairo.h>

typedef struct 
{
    float stx, sty;
} pla_t; /* placement */

int main(int argc, char *argv[])
{

    int i, j;
    int lmar =10, rmar =10, tmar =10, bmar =10;
    int imwidth=640, imheight =480;

    int ncol= 4;
    int nrow= 5;

    pla_t *pla=calloc(ncol*nrow, sizeof(pla_t));

    int awid = imwidth - lmar -rmar; /* (workable) area */
    int ahei = imheight - tmar -bmar;

    float bframe = 10; /* frame around each block */
    float bxtx= (float)(awid/ncol);
    float bxty= (float)(ahei/nrow);
    float xtx= bxtx - 2*bframe;
    float xty= bxty - 2*bframe;


    for(i=0;i<nrow;++i) {
        for(j=0;j<ncol;++j) {
            if(j==0) 
                pla[i*ncol+j].stx = lmar + j*bxtx + 1.5*bframe;
            else
                pla[i*ncol+j].stx = lmar + j*bxtx + .5*bframe;
            if(i==0) 
                pla[i*ncol+j].sty = tmar + i*bxty + 1.5*bframe;
            else
                pla[i*ncol+j].sty = tmar + i*bxty + .5*bframe;
        }
    }

    for(i=0;i<nrow;++i) {
        for(j=0;j<ncol;++j)
            printf("%4.2f/%4.2f/%4.2f/%4.2f ", pla[i*ncol+j].stx, xtx, pla[i*ncol+j].sty, xty);
        printf("\n");
    }

    cairo_surface_t *surface;
    cairo_t *cr;
    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, imwidth, imheight);
    cr = cairo_create (surface);
    cairo_rectangle (cr, 0, 0, imwidth, imheight);
    cairo_set_source_rgba (cr, 0, 0, 0, 0.95);
    cairo_fill (cr);

    for(i=0;i<nrow;++i) {
        for(j=0;j<ncol;++j) {
            cairo_rectangle(cr, pla[i*ncol+j].stx, pla[i*ncol+j].sty, xtx, xty);
            cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
            cairo_fill (cr);
        }
    }
    cairo_surface_write_to_png (surface, "lou0.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    return 0;
}
