/* fit rectangles */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cairo/cairo.h>

typedef struct 
{
    float stx, sty;
} pla_t; /* placement */

int main(int argc, char *argv[])
{
    if(argc!=3) {
        printf("lou1. Two args 1) height of rect 2) width of rect (try 30 20 for a start).");
        exit(EXIT_FAILURE);
    }
    int rhsz=atoi(argv[1]);
    int rvsz=atoi(argv[2]);

    int i, j, k;
    int lmar =60, rmar =10, tmar =80, bmar =40;
    int imwidth=640, imheight =480;
    cairo_surface_t *surface;
    cairo_t *cr;
    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, imwidth, imheight);
    cr = cairo_create (surface);
    cairo_rectangle (cr, 0, 0, imwidth, imheight);
    cairo_set_source_rgba (cr, 0, 0, 0, 0.95);
    cairo_fill (cr);

    int ncol= 12;
    int nrow= 4;

    /* we have an area within the image */
    int awid = imwidth - lmar -rmar; /* (workable) area */
    int ahei = imheight - tmar -bmar;
    cairo_set_source_rgba(cr, .9, .0, .7, 0.9);
    cairo_set_line_width (cr, 0.5);
    cairo_rectangle (cr, lmar, tmar, awid, ahei);
    cairo_stroke(cr);

    pla_t cp;
    cp.stx=lmar+awid/2;
    cp.sty=tmar+ahei/2;
    cairo_set_source_rgba(cr, 0.85, 0.1, 0.2, 0.8);
    cairo_arc(cr, cp.stx, cp.sty, 3, 0, 2 * M_PI);
    cairo_fill(cr);

    float bframe = 10; /* frame around each block */
    cairo_set_source_rgba(cr, .0, 0.8, .0, 0.8);
    cairo_set_line_width (cr, 0.75); /* thinnest really possible */

    /* We're going to take it in 4 quadrants: each quadrant will be overwritten each time */
    pla_t *tbu=calloc(nrow/2*ncol/2, sizeof(pla_t)); /* plus y */
    for(j=0;j<ncol/2;++j) {
        for(i=0;i<nrow/2;++i) {
            tbu[i+j*nrow/2].stx=cp.stx + bframe/2 + j*(bframe+rhsz);
            tbu[i+j*nrow/2].sty=cp.sty - bframe/2 - rvsz - i*(bframe+rvsz);
            cairo_rectangle (cr, tbu[i+j*nrow/2].stx, tbu[i+j*nrow/2].sty, rhsz, rvsz);
            cairo_stroke(cr);
        }
    }
    for(j=0;j<ncol/2;++j) {
        for(i=0;i<nrow/2;++i) {
            tbu[i+j*nrow/2].stx=cp.stx + bframe/2 + j*(bframe+rhsz);
            tbu[i+j*nrow/2].sty=cp.sty + bframe/2 + i*(bframe+rvsz);
            cairo_rectangle (cr, tbu[i+j*nrow/2].stx, tbu[i+j*nrow/2].sty, rhsz, rvsz);
            cairo_stroke(cr);
        }
    }
    /* left quadrants */ 
    for(j=0;j<ncol/2;++j) {
        for(i=0;i<nrow/2;++i) {
            tbu[i+j*nrow/2].stx=cp.stx - bframe/2 -rhsz - j*(bframe+rhsz);
            tbu[i+j*nrow/2].sty=cp.sty - bframe/2 -rvsz - i*(bframe+rvsz);
            cairo_rectangle (cr, tbu[i+j*nrow/2].stx, tbu[i+j*nrow/2].sty, rhsz, rvsz);
            cairo_stroke(cr);
        }
    }
    for(j=0;j<ncol/2;++j) {
        for(i=0;i<nrow/2;++i) {
            tbu[i+j*nrow/2].stx=cp.stx - bframe/2 -rhsz - j*(bframe+rhsz);
            tbu[i+j*nrow/2].sty=cp.sty + bframe/2 + i*(bframe+rvsz);
            cairo_rectangle (cr, tbu[i+j*nrow/2].stx, tbu[i+j*nrow/2].sty, rhsz, rvsz);
            cairo_stroke(cr);
        }
    }

    cairo_surface_write_to_png (surface, "lou1.png");
    free(tbu);
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    return 0;
}
