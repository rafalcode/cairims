/* just generating an index file from vbars[] */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define VBARNUMS 4

int main (int argc, char *argv[])
{
    int i, j, m, *vind, *vpos, width=320, height=240;
    /* vertical divider how ar ewe going to section off the screen vertically */

    float vbar=width/VBARNUMS; /* answer is: equally */
    float *vrpos, *hpos;

    vpos=calloc((1<<VBARNUMS), sizeof(int));
    vrpos=calloc((1<<VBARNUMS), sizeof(float));
    vind=calloc(VBARNUMS+1, sizeof(int));
    hpos=calloc((VBARNUMS), sizeof(float));
    /* arrange array of positions */
    hpos[0]=vbar/2; /* the middle of the first vbar */
    vpos[0]=height/2; /* vert pos, haf way down */
    vind[0]=0;

    for(i=1;i<VBARNUMS;i++) {
        hpos[i] = hpos[i-1] + vbar;
        vind[i] = (1<<i)-1;
        m=0;
        for(j=vind[i-1]; j<vind[i]; ++j) 
            vpos[j] = height/(2<<i) + (m++) * height/(1<<i);
    }

    for(i=0;i<=VBARNUMS;i++)
        printf("%i ", vind[i]);
    printf("\n"); 

    for(i=0;i<(1<<VBARNUMS);i++)
        printf("%i ", vpos[i]);
    printf("\n"); 


    free(hpos);
    free(vpos);
    free(vrpos);
    free(vind);

    return 0;
}
