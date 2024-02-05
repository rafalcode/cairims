/* bincomp, a dbinary competition.
just like a knockout sports event, particularly the NCAA final brackets

 a chief, hidden problem is the c library rand() it's only has 31 random bits. not 32,
 that's because, it's signed. this complicates the random draw somewhat.

 vector is taken in pairs, if 0 the first one if chosen, if 1, he second
 so for the first round, 0 will make odd go thorugh (vec leements are 1-indexed
 and 1 will make evens go through

 beware that the numbe rof random 0's and 1's is half of the vecotr size.
 that's why you'll see /2 coming in there.
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cairo/cairo.h>

#define VBARNUMS 4
#define SQBLX 4
#define SQWD 320 
#define SQHE 240 

#define RSEED 9 // 34 got "chip off the old block"
                 //
#define VSZ 128 // size of our original vector
#define RATAKE 31 // how much to take out of each RAND

int main (int argc, char *argv[])
{
    int i, j;
    
    int *v=malloc(VSZ* sizeof(int));
    for(i=0;i<VSZ;++i) 
        v[i] = i+1;

    int vsz=VSZ; // vector size
    int times = (vsz/2)/RATAKE;
    int raleft = (vsz/2)%RATAKE;

    srand(RSEED);
    int ra;

    int m=0;
    while(vsz>1) {
        printf("New run: "); 
        if(times) {
            for(i=0;i<times;++i) {
                ra=rand();
                for(j=0;j<RATAKE;++j) {
                    v[m]= (0x01 & (ra>>j))?v[2*m+1]:v[2*m];
                    printf("%i ", 0x01 & ra>>j); 
                    m++;
                }
                printf("\n"); 
            }
        }
        if(raleft) {
            ra=rand();
            for(j=0;j<raleft;++j) {
                v[m]= (0x01 & (ra>>j))?v[2*m+1]:v[2*m];
                printf("%i ", 0x01 & ra>>j); 
                m++;
            }
            printf("\n"); 
        }
    
        m=0;
        vsz/=2;
        for(i=0;i<vsz;++i) 
            printf("%i ", v[i]); 
        printf("\n"); 
        printf("vsz: %i\n", vsz); 
        times = (vsz/2)/RATAKE;
        raleft = (vsz/2)%RATAKE;
    }
    // and waht's left:

    free(v);
    return 0;
}
