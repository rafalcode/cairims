/* fit rectangles */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cairo/cairo.h>
const float FONTSZ=10.0; /* the size (number of colours) that our colorscheme has */

typedef struct 
{
    float stx, sty;
} pla_t; /* placement */

void gethcol(float val, float *rgb) // get a heat colour.
{
  // int aR = 0, aG = 0, aB=255;  // RGB for our 1st color (blue in this case).
  // int bR = 255, bG = 0, bB=0;    // RGB for our 2nd color (red in this case).
  float aR = 1.0, aG = .0, aB=.0;
  float bR = .0, bG = 1., bB=.0;    // RGB for our 2nd color (red in this case).

  *rgb   = (float)(bR - aR) * val + aR;      // Evaluated as -255*value + 255.
  *(rgb+1) = (float)(bG - aG) * val + aG;      // Evaluates as 0.
  *(rgb+2)  = (float)(bB - aB) * val + aB;      // Evaluates as 255*value + 0.
}

void gethcol2(float val, float *rgb) // get a heat colour.
{
  // int aR = 0, aG = 0, aB=255;  // RGB for our 1st color (blue in this case).
  // int bR = 255, bG = 0, bB=0;    // RGB for our 2nd color (red in this case).
  float aR = 1.0, aG = .0, aB=.0;
  float bR = .0, bG = 1., bB=.0;    // RGB for our 2nd color (red in this case).

  float rval;
  if( val>0.5) {
      rval=2*(val-.5);
      *rgb   = (float)(bR - aR) * rval + aR;      // Evaluated as -255*value + 255.
      *(rgb+1) = (float)(bG - aG) * rval + aG;      // Evaluates as 0.
      *(rgb+2)  = (float)(bB - aB) * rval + aB;      // Evaluates as 255*value + 0.
  } else {
      *rgb   = 0.3;
      *(rgb+1) = 0.3;
      *(rgb+2)  = 0.3;
  }
}

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

    int ncol= 12, nrow= 8;
    int totv=nrow*ncol;
    float *vala=malloc(totv*sizeof(float));
    for(i=0;i<totv;++i)
        vala[i] = (float)i/(totv-1);
    int totv3=totv*3;
    float *rgba=malloc(totv3*sizeof(float));
    for(i=0;i<totv;i++)
        gethcol(vala[i], rgba+3*i);
    for(i=0;i<totv;i++)
        printf("%4.2f/%4.2f/%4.2f ", rgba[3*i], rgba[3*i+1], rgba[3*i+2]);
    printf("\n"); 

    int nc2=ncol/2, nr2=nrow/2;
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
    float bf2 = bframe/2;
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
    pla_t *tbu=calloc(nr2*nc2, sizeof(pla_t)); /* plus y */
    for(i=0;i<nr2;++i) {
        for(j=0;j<nc2;++j) {
            nij = (nr2-1-i)*nc2+j;
            nij2 = (nc2)*(nr2-i)+nij;
            tbu[nij].stx=cp.stx + bf2 + j*(bframe+rhsz);
            tbu[nij].sty=cp.sty - bf2 - rvsz - i*(bframe+rvsz);
            cairo_set_source_rgba(cr, rgba[3*nij2], rgba[3*nij2+1], rgba[3*nij2+2], 0.8);
            cairo_rectangle (cr, tbu[nij].stx, tbu[nij].sty, rhsz, rvsz);
            cairo_fill(cr);
            sprintf(nstr, "%2.1f%%", 100*vala[nij2]);
            rmp[0] = tbu[nij].stx+rhsz/2;
            rmp[1] = tbu[nij].sty+rvsz/2;
            cairo_text_extents(cr, nstr, &te);
            cairo_move_to (cr, rmp[0] - te.x_bearing - te.width/2, rmp[1] - fe.descent + 1+fe.height/2);
            cairo_set_source_rgba(cr, .7, .7, .7, .8);
            cairo_show_text(cr, nstr);
        }
    }
    for(i=0;i<nr2;++i) {
        for(j=0;j<nc2;++j) {
            nij = i*nc2+j;
            nij2 = ncol*nr2+ (nc2)*(i+1) + nij;
            tbu[nij].stx=cp.stx + bf2 + j*(bframe+rhsz);
            tbu[nij].sty=cp.sty + bf2 + i*(bframe+rvsz);
            cairo_set_source_rgba(cr, rgba[3*nij2], rgba[3*nij2+1], rgba[3*nij2+2], 0.8);
            cairo_rectangle (cr, tbu[nij].stx, tbu[nij].sty, rhsz, rvsz);
            cairo_fill(cr);
            sprintf(nstr, "%2.1f%%", 100*vala[nij2]);
            rmp[0] = tbu[nij].stx+rhsz/2;
            rmp[1] = tbu[nij].sty+rvsz/2;
            cairo_text_extents(cr, nstr, &te);
            cairo_move_to (cr, rmp[0] - te.x_bearing - te.width/2, rmp[1] - fe.descent + 1+fe.height/2);
            cairo_move_to (cr, rmp[0] - te.x_bearing - te.width/2, rmp[1] - fe.descent + 1+fe.height/2);
            cairo_set_source_rgba(cr, .7, .7, .7, .8);
            cairo_show_text(cr, nstr);
        }
    }
    for(i=0;i<nr2;++i) {
        for(j=0;j<nc2;++j) {
            nij = i*nc2+j;
            nij2 = ncol*nr2 + i*ncol+(nc2-j-1);
            tbu[nij].stx=cp.stx - bf2 - rhsz - j*(bframe+rhsz);
            tbu[nij].sty=cp.sty + bf2 + i*(bframe+rvsz);
            cairo_set_source_rgba(cr, rgba[3*nij2], rgba[3*nij2+1], rgba[3*nij2+2], 0.8);
            cairo_rectangle (cr, tbu[nij].stx, tbu[nij].sty, rhsz, rvsz);
            cairo_fill(cr);
            sprintf(nstr, "%2.1f%%", 100*vala[nij2]);
            rmp[0] = tbu[nij].stx+rhsz/2;
            rmp[1] = tbu[nij].sty+rvsz/2;
            cairo_text_extents(cr, nstr, &te);
            cairo_move_to (cr, rmp[0] - te.x_bearing - te.width/2, rmp[1] - fe.descent + 1+fe.height/2);
            cairo_set_source_rgba(cr, .7, .7, .7, .8);
            cairo_show_text(cr, nstr);
        }
    }
    for(i=0;i<nr2;++i) {
        for(j=0;j<nc2;++j) {
            nij = (nr2-1-i)*nc2+j;
            nij2 = (nr2-1-i)*ncol+(nc2-j-1);
            tbu[nij].stx=cp.stx - bf2 - rhsz - j*(bframe+rhsz);
            tbu[nij].sty=cp.sty - bf2 - rvsz - i*(bframe+rvsz);
            cairo_set_source_rgba(cr, rgba[3*nij2], rgba[3*nij2+1], rgba[3*nij2+2], 0.8);
            cairo_rectangle (cr, tbu[nij].stx, tbu[nij].sty, rhsz, rvsz);
            cairo_fill(cr);
            sprintf(nstr, "%2.1f%%", 100*vala[nij2]);
            rmp[0] = tbu[nij].stx+rhsz/2;
            rmp[1] = tbu[nij].sty+rvsz/2;
            cairo_text_extents(cr, nstr, &te);
            cairo_move_to (cr, rmp[0] - te.x_bearing - te.width/2, rmp[1] - fe.descent + 1+fe.height/2);
            cairo_set_source_rgba(cr, .7, .7, .7, .8);
            cairo_show_text(cr, nstr);
        }
    }

    cairo_surface_write_to_png (surface, "lou1.png");
    free(tbu);
    free(rgba);
    free(vala);
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    return 0;
}
