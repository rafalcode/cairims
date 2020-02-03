#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
    float stx, sty;
} pla_t; /* placement */

int main(int argc, char *argv[])
{

    int i, j;
    int lmar =10, rmar =10, tmar =10, bmar =10;
    int imwidth=640, imheight =480;

    int ncol= 3;
    int nrow= 4;

    pla_t *pla=calloc(ncol*nrow, sizeof(pla_t));

    int awid = imwidth - lmar -rmar; /* (workable) area */
    int ahei = imheight - tmar -bmar;

    float bframe = 10; /* frame around each block */
    float bxtx= (float)(awid/ncol) -2*bframe;
    float bxty= (float)(ahei/nrow) -2*bframe;

    for(i=0;i<nrow;++i) {
        for(j=0;j<ncol;++j) {
            if(j==0)
                pla[i*ncol+j].stx = lmar + j*(bxtx + 2*bframe) +bframe/2;
            else if(j==ncol-1)
                pla[i*ncol+j].stx = lmar + j*bxtx + bframe -bframe/2;
            else
                pla[i*ncol+j].stx = lmar + j*bxtx + bframe/2;
        }
    }

    for(i=0;i<nrow;++i) {
        for(j=0;j<ncol;++j)
            printf("%4.2f/%4.2f/%4.2f/%4.2f ", pla[i*ncol+j].stx, pla[i*ncol+j].xtx, pla[i*ncol+j].sty, pla[i*ncol+j].xty);
        printf("\n");
    }

    return 0;
}
