/* trying to sort out waiting time formula */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<sys/time.h>
#include<unistd.h> // for the opts handling and abort()
#include<iostream>
#include<vector>

using namespace std;

#define SETSEED 5 /*  if -DUNPREDRA is not fed in */
#define MX 20

typedef struct /* ev_t, event type: a float (time) and the jump quantity */
{
    float f;
    int j; // jump to be taken at that float time
} ev_t; // event type

typedef struct /* optstruct, a struct for the options */
{
    int uflag; // unpredictble ... i.e. chooses a different randon seed each time.
    char *lfloat;
    char *nnum;
} optstruct;

int catchopts(optstruct *opstru, int oargc, char **oargv)
{
    int c;
    opterr = 0;
    while ((c = getopt (oargc, oargv, "un:l:")) != -1)
        switch (c) {
            case 'u': // want to see the genotypes of these SNPs
                opstru->uflag = 1;
                break;
            case 'n': // print per-sample Tech Rep resolution events
                opstru->nnum = optarg;
                break;
            case 'l': // print per-sample Tech Rep resolution events
                opstru->lfloat = optarg;
                break;
            case '?':
                if( (optopt == 'n') | (optopt =='l') ) {
                    fprintf (stderr, "Option -%c requires a number.\n", optopt);
                    exit(EXIT_FAILURE);
                }
            default:
                abort();
        }
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc<4) {
        printf("Error. Pls supply 2 arguments:\n");
        printf("       -n An integer, number of exponential replicates\n");
        printf("       -l Lambda, an rate, the rate lambda.\n");
        printf("       -u a flag, (optional) for unpredictable random values.\n");
        exit(EXIT_FAILURE);
    }
    int argignore=0; // I know, irrelevant now, but it's how it started: ignoring first few args
    int oargc=argc-argignore;
    char **oargv=argv+argignore;
    optstruct opstru={0};
    catchopts(&opstru, oargc, oargv);

    struct timeval tnow;
    unsigned tseed;
    if(opstru.uflag) {
        gettimeofday(&tnow, NULL);
        /*  use the five last digits of the current microsecond reading to generate a seed */
        tseed=(unsigned int)((tnow.tv_usec/100000.0 - tnow.tv_usec/100000)*RAND_MAX);
        srand(tseed);
    } else
        srand(SETSEED);

    int numreps = atoi(opstru.nnum);
    float lambd = atof(opstru.lfloat);

    int j;
    float ura /*  variable to hold one uniform random variable 0-1 */, cumflt;

    vector<float> eveca[numreps]; // exponen vector array
    vector<ev_t> ev2[numreps]; // exponen vector array

    for(j=0;j<numreps;++j) {
        cumflt=.0;
        do {
            ura= 1. - (float)rand()/(RAND_MAX);
            cumflt += -log(ura)/lambd;
            eveca[j].push_back(cumflt);
            ev2[j].push_back({cumflt, 2});
        } while(cumflt <1.);
    }
    // for(j=0;j<numreps;++j) {
    //     for(float f : eveca[j])
    //         cout << f << ' ';
    //     cout << endl;
    // }
    for(j=0;j<numreps;++j) {
        for(ev_t ev : ev2[j])
            cout << ev.f << '/' << ev.j << ' ';
        cout << endl;
    }

    return 0;
}
