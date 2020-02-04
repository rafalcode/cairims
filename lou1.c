/* fit rectangles */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cairo/cairo.h>
const float FONTSZ=12.0; /* the size (number of colours) that our colorscheme has */

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

    int i, nij, nij2, j, k;
    int lmar =60, rmar =10, tmar =80, bmar =40;
    int imwidth=1600, imheight =900;
    cairo_surface_t *surface;
    cairo_t *cr;
    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, imwidth, imheight);
    cr = cairo_create (surface);
    cairo_rectangle (cr, 0, 0, imwidth, imheight);
    cairo_set_source_rgba (cr, 0, 0, 0, 0.95);
    cairo_fill (cr);

    int ncol= 12;
    int nrow= 6;
    char nstr[32]={0}; // numstrings.

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
    /* OK, now, text consideration */
    cairo_select_font_face (cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size (cr, FONTSZ);
    cairo_font_extents_t fe;
    cairo_font_extents(cr, &fe);
    cairo_text_extents_t te;
    float rmp[2]; // rectangle mid point

    /* We're going to take it in 4 quadrants: each quadrant will be overwritten each time */
    pla_t *tbu=calloc(nrow/2*ncol/2, sizeof(pla_t)); /* plus y */
    for(i=0;i<nrow/2;++i) {
        for(j=0;j<ncol/2;++j) {
            nij = (nrow/2-1-i)*ncol/2+j;
            nij2 = (ncol/2)*(nrow/2-i)+(nrow/2-1-i)*ncol/2+j;
            // nij = i*ncol/2+j;
            tbu[nij].stx=cp.stx + bframe/2 + j*(bframe+rhsz);
            tbu[nij].sty=cp.sty - bframe/2 - rvsz - i*(bframe+rvsz);
            cairo_rectangle (cr, tbu[nij].stx, tbu[nij].sty, rhsz, rvsz);
            cairo_stroke(cr);
            sprintf(nstr, "%i", nij2);
            rmp[0] = tbu[nij].stx+rhsz/2;
            rmp[1] = tbu[nij].sty+rvsz/2;
            cairo_text_extents(cr, nstr, &te);
            cairo_move_to (cr, rmp[0] - te.x_bearing - te.width/2, rmp[1] - fe.descent + 1+fe.height/2);
            cairo_set_source_rgb(cr, 0.9, 0.9, .9);
            cairo_show_text(cr, nstr);
        }
    }
    for(i=0;i<nrow/2;++i) {
        for(j=0;j<ncol/2;++j) {
            nij = i*ncol/2+j;
            nij2 = ncol*nrow/2+ (ncol/2)*(i+1)+i*ncol/2+j;
            tbu[nij].stx=cp.stx + bframe/2 + j*(bframe+rhsz);
            tbu[nij].sty=cp.sty + bframe/2 + i*(bframe+rvsz);
            cairo_rectangle (cr, tbu[nij].stx, tbu[nij].sty, rhsz, rvsz);
            cairo_stroke(cr);
            sprintf(nstr, "%i", nij2);
            rmp[0] = tbu[nij].stx+rhsz/2;
            rmp[1] = tbu[nij].sty+rvsz/2;
            cairo_text_extents(cr, nstr, &te);
            cairo_move_to (cr, rmp[0] - te.x_bearing - te.width/2, rmp[1] - fe.descent + 1+fe.height/2);
            cairo_set_source_rgb(cr, 0.9, 0.9, .9);
            cairo_show_text(cr, nstr);
        }
    }
    for(i=0;i<nrow/2;++i) {
        for(j=0;j<ncol/2;++j) {
            nij = i*ncol/2+j;
            nij2 = ncol*nrow/2 + i*ncol+(ncol/2-j-1);
            // nij2 = ncol*nrow/2+ (ncol/2)*(i+1)+i*ncol/2+(ncol/2-j-1);
            tbu[nij].stx=cp.stx - bframe/2 - rhsz - j*(bframe+rhsz);
            tbu[nij].sty=cp.sty + bframe/2 + i*(bframe+rvsz);
            cairo_rectangle (cr, tbu[nij].stx, tbu[nij].sty, rhsz, rvsz);
            cairo_stroke(cr);
            sprintf(nstr, "%i", nij2);
            rmp[0] = tbu[nij].stx+rhsz/2;
            rmp[1] = tbu[nij].sty+rvsz/2;
            cairo_text_extents(cr, nstr, &te);
            cairo_move_to (cr, rmp[0] - te.x_bearing - te.width/2, rmp[1] - fe.descent + 1+fe.height/2);
            cairo_set_source_rgb(cr, 0.9, 0.9, .9);
            cairo_show_text(cr, nstr);
        }
    }
    // for(j=0;j<ncol/2;++j) {
    //     for(i=0;i<nrow/2;++i) {
    //         tbu[i+j*nrow/2].stx=cp.stx - bframe/2 -rhsz - j*(bframe+rhsz);
    //         tbu[i+j*nrow/2].sty=cp.sty + bframe/2 + i*(bframe+rvsz);
    //         cairo_rectangle (cr, tbu[i+j*nrow/2].stx, tbu[i+j*nrow/2].sty, rhsz, rvsz);
    //         cairo_stroke(cr);
    //     }
    // }
    for(i=0;i<nrow/2;++i) {
        for(j=0;j<ncol/2;++j) {
            nij = (nrow/2-1-i)*ncol/2+j;
            nij2 = (nrow/2-1-i)*ncol+(ncol/2-j-1);
            tbu[nij].stx=cp.stx - bframe/2 - rhsz - j*(bframe+rhsz);
            tbu[nij].sty=cp.sty - bframe/2 - rvsz - i*(bframe+rvsz);
            cairo_rectangle (cr, tbu[nij].stx, tbu[nij].sty, rhsz, rvsz);
            cairo_stroke(cr);
            sprintf(nstr, "%i", nij2);
            rmp[0] = tbu[nij].stx+rhsz/2;
            rmp[1] = tbu[nij].sty+rvsz/2;
            cairo_text_extents(cr, nstr, &te);
            cairo_move_to (cr, rmp[0] - te.x_bearing - te.width/2, rmp[1] - fe.descent + 1+fe.height/2);
            cairo_set_source_rgb(cr, 0.9, 0.9, .9);
            cairo_show_text(cr, nstr);
        }
    }
    // for(i=0;i<nrow/2;++i) {
    //     for(j=0;j<ncol/2;++j) {
    //         nij = i*ncol/2+j;
    //         nij2 = ncol*nrow/2+ (ncol/2)*i+i*ncol/2+j;
    //         tbu[nij].stx=cp.stx - bframe/2 -rhsz - j*(bframe+rhsz);
    //         tbu[nij].sty=cp.sty -rhsz - i*(bframe+rvsz);
    //         cairo_rectangle (cr, tbu[nij].stx, tbu[nij].sty, rhsz, rvsz);
    //         cairo_stroke(cr);
    //         sprintf(nstr, "%i", nij2);
    //         rmp[0] = tbu[nij].stx+rhsz/2;
    //         rmp[1] = tbu[nij].sty+rvsz/2;
    //         cairo_text_extents(cr, nstr, &te);
    //         cairo_move_to (cr, rmp[0] - te.x_bearing - te.width/2, rmp[1] - fe.descent + 1+fe.height/2);
    //         cairo_set_source_rgb(cr, 0.9, 0.9, .9);
    //         cairo_show_text(cr, nstr);
    //     }
    // }

    cairo_surface_write_to_png (surface, "lou1.png");
    free(tbu);
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    return 0;
}
