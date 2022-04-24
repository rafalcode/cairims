/* fit rectangles */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
const float FONTSZ=12.0; /* the size (number of colours) that our colorscheme has */

typedef struct 
{
    float stx, sty;
} pla_t; /* placement */

void gethcol(float val, float *rgb) // get a heat colour.
{
  int aR = 0, aG = 0, aB=255;  // RGB for our 1st color (blue in this case).
  int bR = 255, bG = 0, bB=0;    // RGB for our 2nd color (red in this case).

  *rgb   = (float)(bR - aR) * val + aR;      // Evaluated as -255*value + 255.
  *(rgb+1) = (float)(bG - aG) * val + aG;      // Evaluates as 0.
  *(rgb+2)  = (float)(bB - aB) * val + aB;      // Evaluates as 255*value + 0.
}

int main(int argc, char *argv[])
{

    int i;
    int ncol= 12, nrow= 8;
    int totv=nrow*ncol;
    float *vala=(totv*sizeof(float));
    for(i=0;i<totv;++i)
        vala[i] = (float)i/totv;
    int totv3=totv*3;
    float *rgba=malloc(totv3*sizeof(float));
    for(i=0;i<totv;i++)
        gethcol(vala[i], rgba+3*i);
    for(i=0;i<totv;i++)
        printf("%4.2f/%4.2f/%4.2f ", rgba[3*i], rgba[3*i+1], rgba[3*i+2]);
    printf("\n"); 

    free(vala);
        free(rgba);
    return 0;
}
