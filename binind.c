/* just generating an index file from hbars[] */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define VBARNUMS 6

int main (int argc, char *argv[])
{
    int i, *vind, width=480, height=480;

    /* vertical divider how ar ewe going to section off the screen vertically */
    float hbar=width/VBARNUMS; /* answer is: equally */
    float *hpos;
    vind=calloc((2*VBARNUMS), sizeof(int));
    hpos=calloc((VBARNUMS), sizeof(float));
    /* arrange array of positions */
    hpos[0]=hbar/2;
    vind[0]=0;
    for(i=1;i<VBARNUMS;i++) {
        hpos[i] = hpos[i-1] + hbar;
        vind[2*i] = (1<<i)-1;
        vind[2*i+1] = ((2<<i)-1);
    }

    for(i=0;i<VBARNUMS;i++)
        printf("%i - %i | ", vind[2*i], vind[2*i+1]);
    printf("\n"); 

    free(hpos);
    free(vind);

    return 0;
}
