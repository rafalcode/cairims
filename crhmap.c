/* fit rectangles */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cairo/cairo.h>
#include "csvrd.h"
const float FONTSZ=10.0; /* the size (number of colours) that our colorscheme has */

typedef struct 
{
    float stx, sty;
} pla_t; /* placement */

w_c *crea_wc(unsigned initsz)
{
    w_c *wc=malloc(sizeof(w_c));
    wc->lp1=initsz;
    wc->t=STRG;
    wc->w=malloc(wc->lp1*sizeof(char));
    return wc;
}

void reall_wc(w_c **wc, unsigned *cbuf)
{
    w_c *twc=*wc;
    unsigned tcbuf=*cbuf;
    tcbuf += CBUF;
    twc->lp1=tcbuf;
    twc->w=realloc(twc->w, tcbuf*sizeof(char));
    *wc=twc; /* realloc can often change the ptr */
    *cbuf=tcbuf;
    return;
}

void norm_wc(w_c **wc)
{
    w_c *twc=*wc;
    twc->w=realloc(twc->w, twc->lp1*sizeof(char));
    *wc=twc; /* realloc can often change the ptr */
    return;
}

void free_wc(w_c **wc)
{
    w_c *twc=*wc;
    free(twc->w);
    free(twc);
    return;
}

aw_c *crea_awc(unsigned initsz)
{
    int i;
    aw_c *awc=malloc(sizeof(aw_c));
    awc->ab=initsz;
    awc->al=awc->ab;
    awc->aw=malloc(awc->ab*sizeof(w_c*));
    for(i=0;i<awc->ab;++i) 
        awc->aw[i]=crea_wc(CBUF);
    return awc;
}

void reall_awc(aw_c **awc, unsigned buf)
{
    int i;
    aw_c *tawc=*awc;
    tawc->ab += buf;
    tawc->al=tawc->ab;
    tawc->aw=realloc(tawc->aw, tawc->ab*sizeof(aw_c*));
    for(i=tawc->ab-buf;i<tawc->ab;++i)
        tawc->aw[i]=crea_wc(CBUF);
    *awc=tawc;
    return;
}

void norm_awc(aw_c **awc)
{
    int i;
    aw_c *tawc=*awc;
    /* free the individual w_c's */
    for(i=tawc->al;i<tawc->ab;++i) 
        free_wc(tawc->aw+i);
    /* now release the pointers to those freed w_c's */
    tawc->aw=realloc(tawc->aw, tawc->al*sizeof(aw_c*));
    *awc=tawc;
    return;
}

void free_awc(aw_c **awc)
{
    int i;
    aw_c *tawc=*awc;
    for(i=0;i<tawc->al;++i) 
        free_wc(tawc->aw+i);
    free(tawc->aw); /* unbelieveable: I left this out, couldn't find where I leaking the memory! */
    free(tawc);
    return;
}

aaw_c *crea_aawc(unsigned initsz)
{
    int i;
    unsigned lbuf=initsz;
    aaw_c *aawc=malloc(sizeof(aaw_c));
    aawc->numl=0;
    aawc->aaw=malloc(lbuf*sizeof(aw_c*));
    for(i=0;i<initsz;++i) 
        aawc->aaw[i]=crea_awc(WABUF);
    return aawc;
}

void free_aawc(aaw_c **aw)
{
    int i;
    aaw_c *taw=*aw;
    for(i=0;i<taw->numl;++i) /* tried to release 1 more, no go */
        free_awc(taw->aaw+i);
    free(taw->aaw);
    free(taw);
}

void prtaawcdbg(aaw_c *aawc)
{
    int i, j, k;
    for(i=0;i<aawc->numl;++i) {
        printf("l.%u(%u): ", i, aawc->aaw[i]->al); 
        for(j=0;j<aawc->aaw[i]->al;++j) {
            printf("w_%u: ", j); 
            if(aawc->aaw[i]->aw[j]->t == NUMS) {
                printf("NUM! "); 
                continue;
            } else if(aawc->aaw[i]->aw[j]->t == PNI) {
                printf("PNI! "); 
                continue;
            } else if(aawc->aaw[i]->aw[j]->t == STCP) {
                printf("STCP! "); 
                continue;
            }
            for(k=0;k<aawc->aaw[i]->aw[j]->lp1-1; k++)
                putchar(aawc->aaw[i]->aw[j]->w[k]);
            printf("/%u ", aawc->aaw[i]->aw[j]->lp1-1); 
        }
        printf("\n"); 
    }
}

void prtaawcdata(aaw_c *aawc) /* print line and word details, but not the words themselves */
{
    int i, j;
    for(i=0;i<aawc->numl;++i) {
        printf("L%u(%uw):", i, aawc->aaw[i]->al); 
        for(j=0;j<aawc->aaw[i]->al;++j) {
            printf("l%ut", aawc->aaw[i]->aw[j]->lp1-1);
            switch(aawc->aaw[i]->aw[j]->t) {
                case NUMS: printf("N "); break;
                case PNI: printf("I "); break;
                case STRG: printf("S "); break;
                case STCP: printf("C "); break; /* closing punctuation */
                case SCST: printf("Z "); break; /* starting capital */
                case SCCP: printf("Y "); break; /* starting capital and closing punctuation */
                case ALLC: printf("A "); break; /* horrid! all capitals */
            }
        }
    }
    printf("\n"); 
    printf("L is a line, l is length of word, S is normal string, C closing punct, Z, starting cap, Y Starting cap and closing punct.\n"); 
}

void prtaawcp5(aaw_c *aawc) /* print line and word details, but not the words themselves */
{
    int i, j;
    for(i=3;i<aawc->numl;++i) {
        j=(i-3)%12;
        printf((j!=11)?"%s ":"%s\n", aawc->aaw[i]->aw[4]->w);
    }
}

aaw_c *processinpf(char *fname)
{
    /* declarations */
    FILE *fp=fopen(fname,"r");
    int i;
    size_t couc /*count chars per line */, couw=0 /* count words */;
    int c, oldc='\0';
    boole inword=0;
    unsigned lbuf=LBUF /* buffer for number of lines */, cbuf=CBUF /* char buffer for size of w_c's: reused for every word */;
    aaw_c *aawc=crea_aawc(lbuf); /* array of words per line */

    while( (c=fgetc(fp)) != EOF) {
        if( (c== '\n') | (c == ' ') | (c == '\t') ) {
            if( inword==1) { /* cue word-ending procedure */
                aawc->aaw[aawc->numl]->aw[couw]->w[couc++]='\0';
                aawc->aaw[aawc->numl]->aw[couw]->lp1=couc;
                SETCPTYPE(oldc, aawc->aaw[aawc->numl]->aw[couw]->t);
                norm_wc(aawc->aaw[aawc->numl]->aw+couw);
                couw++; /* verified: this has to be here */
            }
            if(c=='\n') { /* cue line-ending procedure */
                if(aawc->numl ==lbuf-1) {
                    lbuf += LBUF;
                    aawc->aaw=realloc(aawc->aaw, lbuf*sizeof(aw_c*));
                    for(i=lbuf-LBUF; i<lbuf; ++i)
                        aawc->aaw[i]=crea_awc(WABUF);
                }
                aawc->aaw[aawc->numl]->al=couw;
                norm_awc(aawc->aaw+aawc->numl);
                aawc->numl++;
                couw=0;
            }
            inword=0;
        } else if(inword==0) { /* a normal character opens word */
            if(couw ==aawc->aaw[aawc->numl]->ab-1) /* new word opening */
                reall_awc(aawc->aaw+aawc->numl, WABUF);
            couc=0;
            cbuf=CBUF;
            aawc->aaw[aawc->numl]->aw[couw]->w[couc++]=c;
            GETLCTYPE(c, aawc->aaw[aawc->numl]->aw[couw]->t); /* MACRO: the firt character gives a clue */
            inword=1;
        } else if(inword) { /* simply store */
            if(couc == cbuf-1)
                reall_wc(aawc->aaw[aawc->numl]->aw+couw, &cbuf);
            aawc->aaw[aawc->numl]->aw[couw]->w[couc++]=c;
            /* if word is a candidate for a NUM or PNI (i.e. via its first character), make sure it continues to obey rules: a MACRO */
            IWMODTYPEIF(c, aawc->aaw[aawc->numl]->aw[couw]->t);
        }
        oldc=c;
    } /* end of big for statement */
    fclose(fp);

    /* normalization stage */
    for(i=aawc->numl; i<lbuf; ++i) {
        free_awc(aawc->aaw+i);
    }
    aawc->aaw=realloc(aawc->aaw, aawc->numl*sizeof(aw_c*));

    return aawc;
}

aaw_c *processincsv(char *fname)
{
    /* declarations */
    FILE *fp=fopen(fname,"r");
    int i;
    size_t couc /*count chars per line */, couw=0 /* count words */;
    int c;
    boole inword=0;
    unsigned lbuf=LBUF /* buffer for number of lines */, cbuf=CBUF /* char buffer for size of w_c's: reused for every word */;
    aaw_c *aawc=crea_aawc(lbuf); /* array of words per line */

    while( (c=fgetc(fp)) != EOF) {
        if( (c== '\n') | (c == ',') ) {
            if( inword==1) { /* cue word-ending procedure */
                aawc->aaw[aawc->numl]->aw[couw]->w[couc++]='\0';
                aawc->aaw[aawc->numl]->aw[couw]->lp1=couc;
                norm_wc(aawc->aaw[aawc->numl]->aw+couw);
                couw++; /* verified: this has to be here */
            }
            if(c=='\n') { /* cue line-ending procedure */
                if(aawc->numl ==lbuf-1) {
                    lbuf += LBUF;
                    aawc->aaw=realloc(aawc->aaw, lbuf*sizeof(aw_c*));
                    for(i=lbuf-LBUF; i<lbuf; ++i)
                        aawc->aaw[i]=crea_awc(WABUF);
                }
                aawc->aaw[aawc->numl]->al=couw;
                norm_awc(aawc->aaw+aawc->numl);
                aawc->numl++;
                couw=0;
            }
            inword=0;
        } else if(inword==0) { /* a normal character opens word */
            if(couw ==aawc->aaw[aawc->numl]->ab-1) /* new word opening */
                reall_awc(aawc->aaw+aawc->numl, WABUF);
            couc=0;
            cbuf=CBUF;
            aawc->aaw[aawc->numl]->aw[couw]->w[couc++]=c;
            inword=1;
        } else if(inword) { /* simply store */
            if(couc == cbuf-1)
                reall_wc(aawc->aaw[aawc->numl]->aw+couw, &cbuf);
            aawc->aaw[aawc->numl]->aw[couw]->w[couc++]=c;
            /* if word is a candidate for a NUM or PNI (i.e. via its first character), make sure it continues to obey rules: a MACRO */
        }
    } /* end of big for statement */
    fclose(fp);

    /* normalization stage */
    for(i=aawc->numl; i<lbuf; ++i) {
        free_awc(aawc->aaw+i);
    }
    aawc->aaw=realloc(aawc->aaw, aawc->numl*sizeof(aw_c*));

    return aawc;
}

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
    float cR = .4, cG = .8, cB=.0;    // RGB for our 2nd color (red in this case).
    float rval;
    float thresh[2]={0.8, 0.9};
    if(val<0.9) {
        *rgb   = .9; *(rgb+1) = .1; *(rgb+2)  = 0;
    } else if(val<0.94) {
        *rgb   = .8; *(rgb+1) = 0.3; *(rgb+2)  = .1;
    } else if(val<0.98) {
        *rgb   = .4; *(rgb+1) = .8; *(rgb+2)  = .2;
    } else {
        *rgb   = .1; *(rgb+1) = .9; *(rgb+2)  = .1;
    }
}

int main(int argc, char *argv[])
{
    if(argc!=4) {
        printf("crhmap. Three args 1) DNAreport csv 2) height of rect 3) width of rect (try 30 20 for a start).");
        exit(EXIT_FAILURE);
    }
    aaw_c *aawc=processincsv(argv[1]);
    int rw=atoi(argv[2]);
    int rh=atoi(argv[3]);

    int i, nij, j;
    int lmar =10, rmar =10, tmar =40, bmar =10;
    int imwidth=1024, imheight =768;
    //
    // int imwidth=800, imheight =600;
    int awid = imwidth - lmar -rmar; /* (workable) area */
    int ahei = imheight - tmar -bmar;
    char *tc;

    cairo_surface_t *surface;
    cairo_t *cr;
    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, imwidth, imheight);
    cr = cairo_create (surface);
    cairo_rectangle (cr, 0, 0, imwidth, imheight);
    cairo_set_source_rgba (cr, 0, 0, 0, 0.95);
    cairo_fill (cr);
    cairo_select_font_face (cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size (cr, 16.0);
    cairo_font_extents_t fe;
    cairo_font_extents(cr, &fe);
    cairo_text_extents_t te;
    cairo_move_to (cr, lmar - te.x_bearing - te.width/2, tmar/2 - fe.descent + 1+ fe.height);
    cairo_set_source_rgb(cr, .6, .6, .6);
    tc=strrchr(argv[1], '/');
    cairo_show_text(cr, tc+1);

    int ncol= 12;
    int nrow = (aawc->numl-3)/ncol;
    int totv=nrow*ncol;
    printf("totv=%i\n", totv); 
    float *vala=malloc(totv*sizeof(float));
    for(i=0;i<totv;++i) {
        vala[i] = atof(aawc->aaw[i+3]->aw[4]->w);
        printf("%2.2f ", vala[i]);
    }
    printf("\n"); 
    int totv3=totv*3;
    float *rgba=malloc(totv3*sizeof(float));
    for(i=0;i<totv;i++)
        gethcol2(vala[i], rgba+3*i);
    for(i=0;i<totv;i++)
        printf("%4.2f/%4.2f/%4.2f ", rgba[3*i], rgba[3*i+1], rgba[3*i+2]);
    printf("\n"); 

    int nc2=ncol/2, nr2=nrow/2;
    char nstr[32]={0}; // numstrings.

    /* we have an area within the image
    cairo_set_source_rgba(cr, .9, .0, .7, 0.9);
    cairo_set_line_width (cr, 0.5);
    cairo_rectangle (cr, lmar, tmar, awid, ahei);
    cairo_stroke(cr);
    */

    float bframe = 10; /* frame around each block */
    float bf2 = bframe/2;
    pla_t kp; // keypoint, find centrepoint and subtract
    kp.stx=(lmar+awid/2) - nc2*(bf2 + rw);
    kp.sty=(tmar+ahei/2) - nr2*(bf2 + rh);

    cairo_set_source_rgba(cr, .0, 0.8, .0, 0.8);
    cairo_set_line_width (cr, 0.75); /* thinnest really possible */
    /* OK, now, text consideration */
    cairo_set_font_size (cr, FONTSZ);
    cairo_font_extents(cr, &fe);
    float rmp[2]; // rectangle mid point
    char *tc0, tc2;

    /* We're going to take it in 4 quadrants: each quadrant will be overwritten each time */
    pla_t *tbu=calloc(nrow*ncol, sizeof(pla_t)); /* plus y */
    for(i=0;i<nrow;++i) {
        for(j=0;j<ncol;++j) {
            nij = ncol*i+j;
            tbu[nij].stx=kp.stx + j*(bframe+rw);
            tbu[nij].sty=kp.sty + i*(bframe+rh);
            cairo_set_source_rgba(cr, rgba[3*nij], rgba[3*nij+1], rgba[3*nij+2], 0.8);
            cairo_rectangle (cr, tbu[nij].stx, tbu[nij].sty, rw, rh);
            cairo_fill(cr);
            sprintf(nstr, "%2.1f%%", 100*vala[nij]);
            rmp[0] = tbu[nij].stx+rw/2;
            rmp[1] = tbu[nij].sty+rh/2;
            cairo_text_extents(cr, nstr, &te);
            // cairo_move_to (cr, rmp[0] - te.x_bearing - te.width/2, rmp[1] - fe.descent + 1+fe.height/2);
            cairo_move_to (cr, rmp[0] - te.x_bearing - te.width/2, rmp[1] - fe.descent);
            cairo_set_source_rgb(cr, .0, .0, .0);
            cairo_show_text(cr, nstr);
            cairo_text_extents(cr, aawc->aaw[nij+3]->aw[1]->w, &te);
            cairo_move_to (cr, rmp[0] - te.x_bearing - te.width/2, rmp[1] - fe.descent + 1+ fe.height);
            cairo_show_text(cr, aawc->aaw[nij+3]->aw[1]->w);
        }
    }

    cairo_surface_write_to_png (surface, "crhmap2.png");
    free(tbu);
    free(rgba);
    free(vala);
    free_aawc(&aawc);
    cairo_destroy(cr);
    cairo_surface_destroy (surface);

    return 0;
}
