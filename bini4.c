/* just generating an index file from hbars[] */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define VBARNUMS 4

int main (int argc, char *argv[])
{
    int i, j, m, k, n, *vind, width=320, height=240;
    /* vertical divider how ar ewe going to section off the screen vertically */

    float hbar=width/VBARNUMS; /* answer is: equally */
    float *vpos, *hpos;

for(i=0;i<N;++i)    
    vpos=calloc((1<<VBARNUMS), sizeof(float));
    vind=calloc((2*VBARNUMS), sizeof(int));
    hpos=calloc((VBARNUMS), sizeof(float));
    /* arrange array of positions */
    hpos[0]=k*WID/2 +hbar/2;
    vind[0]=n*HEI/2+ height/2;
    for(i=1;i<VBARNUMS;i++) {
        hpos[i] = hpos[i-1] + hbar;
        vind[2*i] = (1<<i)-1;
        vind[2*i+1] = ((2<<i)-1);
        m=0;
        for(j=vind[2*i]; j<vind[2*i+1]; ++j) 
            vpos[j] = n*HEI/2 + height/(2<<i) + (m++) * height/(1<<i);
    }

    for(i=0;i<VBARNUMS;i++)
        printf("%i - %i | ", vind[2*i], vind[2*i+1]);
    printf("\n"); 
    printf("Vpositions printing as single array:\n"); 
    for(i=0;i<1<<VBARNUMS;++i) {
        printf("%3.4f ",vpos[i]); 
    }
    printf("Vpositions as single array, but reversed:\n"); 
    for(i=(1<<VBARNUMS)-1;i>=0;--i) {
        printf("%3.4f ",vpos[i]); 
    }
    printf("\n"); 
    printf("Vpositions printed with newlines where there's a change in hpos:\n"); 
    for(i=1;i<VBARNUMS;i++) {
        printf("hpos: %3.4f -> ",hpos[i]); 
        for(j=vind[2*i]; j<vind[2*i+1]; ++j) 
            printf("%3.4f ",vpos[j]); 
        printf("\n"); 
    }
    free(hpos);
    free(vpos);
    free(vind);

    return 0;
}
