/* just generating an index file from vbars[] */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define VBARNUMS 4

int main (int argc, char *argv[])
{
    int i, j, m, *vind /* vertical indices */, width=320, height=240;
    /* vertical divider how ar ewe going to section off the screen vertically */

    float vbar=width/VBARNUMS; /* answer is: equally */
    float *vpos, *hpos;

    vpos=calloc((1<<VBARNUMS), sizeof(float));
    vind=calloc((2*VBARNUMS), sizeof(int));
    hpos=calloc((VBARNUMS), sizeof(float));
    /* arrange array of positions */
    hpos[0]=vbar/2; /* the middle of the first vbar */
    vpos[0]=height/2; /* vert pos, haf way down */
    vind[0]=0;

    for(i=1;i<VBARNUMS;i++) {
        hpos[i] = hpos[i-1] + vbar;
        vind[2*i-1] = (1<<i)-1;
        vind[2*i] = ((2<<i)-1);
        m=0;
        for(j=vind[2*i-1]; j<vind[2*i]; ++j) 
            vpos[j] = height/(2<<i) + (m++) * height/(1<<i);
    }

    for(i=0;i<VBARNUMS;i++)
        printf("%i - %i | ", vind[2*i], vind[2*i+1]);
    printf("\n"); 
    printf("Vpositions printing as single array:\n"); 
    for(i=0;i<(1<<VBARNUMS)-1;++i) {
        printf("%3.4f ",vpos[i]); 
    }
    printf("\n"); 
    printf("Vpositions as single array, but reversed:\n"); 
    for(i=(1<<VBARNUMS)-2;i>=0;--i) { /*  note that minus 2 */
        printf("%3.4f ",vpos[i]); 
    }
    printf("\n"); 
    printf("Vpositions printed with newlines where there's a change in hpos:\n"); 
    /*  first position is an exception */
        printf("hpos: %3.4f -> %3.4f\n",hpos[0], vpos[0]); 
    for(i=1;i<VBARNUMS;i++) {
        printf("hpos: %3.4f -> ",hpos[i]); 
        for(j=vind[2*i-1]; j<vind[2*i]; ++j) 
            printf("%3.4f ",vpos[j]); 
        printf("\n"); 
    }
    free(hpos);
    free(vpos);
    free(vind);

    return 0;
}
