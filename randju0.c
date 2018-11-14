/* trying to sort out waiting time formula */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define SETSEED 5 /*  if -DUNPREDRA is not fed in */
#define MX 20

int rajuhh(int quanju) /* not equl odds, higher is rarer, of course */
{
    int i;
    int totsum=quanju*(quanju+1)/2; // typical n(n+1)/2 formula */
    float *cats=calloc(quanju-1, sizeof(float));
    cats[0]=(float)quanju/totsum;
    for(i=1;i<quanju-1;++i)
        cats[i]=cats[i-1]+(float)(quanju-i)/totsum;
    
    float ura= 1. - (float)rand()/(RAND_MAX);
#if DBG
    printf("ura=%4.6f\n", ura); 
#endif

    for(i=0;i<quanju-1;++i)
        if(ura < cats[i]) {
            free(cats);
            return i+1;
        }

    // if you reach this point, then it's the max category
    free(cats);
    return i+1;
}

int raju(int quanju) /* equal odds for all categories, not so interesting as higher is better */
{
    int i;
    float *cats=calloc(quanju-1, sizeof(float));
    float gap=1./quanju;
    cats[0]=gap;
    for(i=1;i<quanju;++i) 
        cats[i] = gap + cats[i-1];

    float ura= 1. - (float)rand()/(RAND_MAX);
    printf("ura=%4.6f\n", ura); 

    for(i=0;i<quanju;++i)
        if(ura < cats[i])
            return i+1;

    // if you reach this point, then it's the max category
    return i+1;
}

int main(int argc, char *argv[])
{
    if(argc!=3) {
        printf("Error. Pls supply 2 args: 1) number of categories 2) number of replicates.\n");
        exit(EXIT_FAILURE);
    }
    int i;
    int quanju=atoi(argv[1]);
    int numreps=atoi(argv[2]);
    printf("Higher-is-harder %i-sided dice throws:\n", quanju);
    for(i=0;i<numreps;++i) 
        printf("%i ", rajuhh(quanju));
    printf("\n"); 
    return 0;
}
