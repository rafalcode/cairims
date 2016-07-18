/* just generating an index file from vbars[] */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define VBARNUMS 4


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
    int i, j, m, *vind, *rvind, *vpos, width=320, height=240;
    /* vertical divider how ar ewe going to section off the screen vertically */

    vpos=calloc((1<<VBARNUMS), sizeof(int));
    vind=calloc(VBARNUMS+1, sizeof(int));
    /* arrange array of positions */
    vind[0]=0;

    for(i=0;i<VBARNUMS;i++) {
        vind[i+1] = (1<<(i+1))-1;
        m=0;
        for(j=vind[i]; j<vind[i+1]; ++j) 
            vpos[j] = height/(2<<i) + (m++) * height/(1<<i);
    }

    for(i=0;i<=VBARNUMS;i++)
        printf("%i ", vind[i]);
    printf("\n"); 

    for(i=0;i<(1<<VBARNUMS)-1;i++)
        printf("%i ", vpos[i]);
    printf("\n"); 
    printf("Or:\n"); 
    for(i=0;i<VBARNUMS;i++) {
        for(j=vind[i]; j<vind[i+1]; ++j) 
            printf("%i ", vpos[j]);
        printf("\n"); 
    }
//     printf("\nNow parse it in reverse 1st att:\n"); 
//     for(i=0;i<VBARNUMS;i++) {
//         m=VBARNUMS-i;
//         for(j=vind[i]; j>(vind[m]-vind[m-1]); ++j) 
//             printf("%i ", vpos[j]);
//         printf("\n"); 
//     }

    printf("\nNow parse it in reverse 2nt att:\n"); 
    rvind=crearvarr(vind, VBARNUMS+1);

    for(i=0;i<=VBARNUMS;i++)
        printf("%i ", rvind[i]);
    printf("\n"); 

   for(i=0;i<VBARNUMS;i++) {
       for(j=rvind[i]-1; j>=rvind[i+1]; --j) 
           printf("%i ", vpos[j]+240);
       /*  here I'm adding an offset to the vpos, so they all get pushed down, as well as reversed */
       printf("\n"); 
   }

    free(vpos);
    free(vind);
    free(rvind);

    return 0;
}
