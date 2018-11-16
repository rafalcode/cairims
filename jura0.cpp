/* jump race: fixed at two participants for the time being. */

/* implementation notes:
 * note how the tohere treatment, as usual a little stealthy
 * if the latest next event time indexed by ii, remains lower
 * than the increment tp, then it uses the old tohere, as in nothing changes.
 * if tp goes over this time, the tohere changes and we move up an index on ii to the next event time for that replicate
 * so there's minimal touching of the tohere value.
 *
 * Running notee
 * ./jura0_d -s 5 -j 3 -l 3 -u
 *  this seems to go too fast
 *  and despite landa being 3, there seems to be way more jumps on average over the distance
 *  so something not great still
 *
 *  TODO:
 *  Put circles in the stop positions, it helps
 */
#include<cairo/cairo.h>
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<dirent.h> 
#include<iostream>
#include<vector>

#define FPSEC 25 // change this to 30 if required.
#define FNAMSZ 64 // for the output file name
#define SYSCMD 128
#define FONTSZ 16
#define LMAR 20
#define JUSZ .05 // size that a single jump should be as a fraction of the course.

#define SETSEED 5 /*  if -DUNPREDRA is not fed in */

using namespace std;

typedef struct /* colstru_f, color struct */
{
    char nam[FNAMSZ];
    float rgb[3];
} colstru_f;

typedef struct /* ev_t, event type: a float (time) and the jump quantity */
{
    float f;
    float j; // jump to be taken at that float time
} ev_t; // event type

typedef struct /* optstruct, a struct for the options */
{
    int uflag; // unpredictble ... i.e. chooses a different randon seed each time.
    char *lfloat;
    char *snum;
    char *jnum;
} optstruct;

int catchopts(optstruct *opstru, int oargc, char **oargv)
{
    int c;
    opterr = 0;
    while ((c = getopt (oargc, oargv, "us:l:j:")) != -1)
        switch (c) {
            case 'u': // want to see the genotypes of these SNPs
                opstru->uflag = 1;
                break;
            case 's': // duration.
                opstru->snum = optarg;
                break;
            case 'j': // print per-sample Tech Rep resolution events
                opstru->jnum = optarg;
                break;
            case 'l': // print per-sample Tech Rep resolution events
                opstru->lfloat = optarg;
                break;
            case '?':
                if( (optopt == 'n') | (optopt =='l') | (optopt == 'j')) {
                    fprintf (stderr, "Option -%c requires a number.\n", optopt);
                    exit(EXIT_FAILURE);
                }
            default:
                abort();
        }
    return 0;
}

float *juint2(int mxju) /* not equl odds, higher is harder/rarer, harsher version, sum of squares */
{
    int i;
    int totsum2=0;
    for(i=1;i<=mxju;++i)
        totsum2 += pow(i,2);
    float *cats=new (nothrow) float[mxju-1];
    cats[0]=(float)pow(mxju,2)/totsum2;
    for(i=1;i<mxju-1;++i)
        cats[i]=cats[i-1]+(float)pow(mxju-i,2)/totsum2;

    return cats;
}

float *juint0(int mxju) /* jump intervals: not equl odds, higher is rarer, of course */
{
    int i;
    int totsum=mxju*(mxju+1)/2; // typical n(n+1)/2 formula */
    float *cats=new (nothrow) float[mxju-1];
    cats[0]=(float)mxju/totsum;
    for(i=1;i<mxju-1;++i)
        cats[i]=cats[i-1]+(float)(mxju-i)/totsum;

    return cats;
}

int rajuhh0(int mxju)
{
    int i;
    float *cats=juint0(mxju);
    float ura= 1. - (float)rand()/(RAND_MAX);
#if DBG
    printf("ura=%4.6f\n", ura); 
#endif

    for(i=0;i<mxju-1;++i)
        if(ura < cats[i]) {
            delete[] cats;
            return i+1;
        }

    delete[] cats;
    // if you reach this point, then it's the max category
    return i+1;
}

int rajuhh(int mxju) /* not equl odds, higher is rarer, of course */
{
    int i;
    int totsum=mxju*(mxju+1)/2; // typical n(n+1)/2 formula */
    float *cats=new (nothrow) float[mxju-1];
    cats[0]=(float)mxju/totsum;
    for(i=1;i<mxju-1;++i)
        cats[i]=cats[i-1]+(float)(mxju-i)/totsum;

    float ura= 1. - (float)rand()/(RAND_MAX);
#if DBG
    printf("ura=%4.6f\n", ura); 
#endif

    for(i=0;i<mxju-1;++i)
        if(ura < cats[i]) {
            delete[] cats;
            return i+1;
        }

    // if you reach this point, then it's the max category
    delete[] cats;
    return i+1;
}

vector<ev_t> *get_race(int numreps /* 2 to start with */, int mxju, float lambd, int courselen)
{
    int j;
    float ura /*  variable to hold one uniform random variable 0-1 */, cumflt, cumjuf;
    vector<ev_t> *ev2=new (nothrow) vector<ev_t>[numreps];

    for(j=0;j<numreps;++j) {
        cumflt=.0;
        cumjuf=.0;
        do {
            ura= 1. - (float)rand()/(RAND_MAX);
            cumflt += -log(ura)/lambd;
            cumjuf += JUSZ * courselen * rajuhh0(mxju);
            ev2[j].push_back({cumflt, cumjuf});
        } while(cumjuf < courselen);
    }

    return ev2;
}

char *mktmpd(void)
{
    struct timeval tsecs;
    gettimeofday(&tsecs, NULL);
    char *myt=(char*)calloc(FNAMSZ, sizeof(char));
    sprintf(myt, "tmpdir_%lu", tsecs.tv_usec);

    DIR *d;
    while((d = opendir(myt)) != NULL) { /* see if a directory witht the same name exists ... very low likelihood though */
        gettimeofday(&tsecs, NULL);
        sprintf(myt, "tmpdir_%lu", tsecs.tv_usec);
        closedir(d);
    }
    closedir(d);
    mkdir(myt, S_IRWXU);

    return myt;
}

void prtvec(vector<ev_t>* ev2, int numreps)
{
    int j;
    for(j=0;j<numreps;++j) {
        printf("Rep %i) ", j+1); 
        for(ev_t ev : ev2[j])
            // cout << 'At time ' << ev.f << ', ' << ev.j << ' jumps / ';
            printf("f:%4.4f/j:%4.4f ", ev.f, ev.j);
         printf("\n"); 
    }
    return;
}

void prtusage(void)
{
    printf("jura0: jump race. Arguments as options as follows:\n");
    printf("       -u, flag for unpredictable random values.\n");
    printf("       -l, float, lambda value, average number of (jump) events per run.\n");
    printf("       -s, integer, duration in seconds.\n");
    printf("       -j, integer, max number of jumps achieveable in single jump event.\n");
    printf("       -l, -n, and -j are obligatory.\n");
    exit(EXIT_FAILURE);
}

int main (int argc, char *argv[])
{
    colstru_f colsf[18] = { {"foreground", {0.866667, 0.866667, 0.866667} }, {"background", {0.066667, 0.066667, 0.066667} }, {"color0", {0.098039, 0.098039, 0.098039} }, {"color8", {0.627451, 0.321569, 0.176471} }, {"color1", {0.501961, 0.196078, 0.196078} }, {"color9", {0.596078, 0.168627, 0.168627} }, {"color2", {0.356863, 0.462745, 0.184314} }, {"color10", {0.537255, 0.721569, 0.247059} }, {"color3", {0.666667, 0.600000, 0.262745} }, {"color11", {0.937255, 0.937255, 0.376471} }, {"color4", {0.196078, 0.298039, 0.501961} }, {"color12", {0.168627, 0.309804, 0.596078} }, {"color5", {0.439216, 0.423529, 0.603922} }, {"color13", {0.509804, 0.415686, 0.694118} }, {"color6", {0.572549, 0.694118, 0.619608} }, {"color14", {0.631373, 0.803922, 0.803922} }, {"color7", {1.000000, 1.000000, 1.000000} }, {"color15", {0.866667, 0.866667, 0.866667} } };

    if(argc<7)
        prtusage();

    int argignore=0; // I know, irrelevant now, but it's how it started: ignoring first few args
    int oargc=argc-argignore;
    char **oargv=argv+argignore;
    optstruct opstru={0};
    catchopts(&opstru, oargc, oargv);

    // the obligatory options
    if( (opstru.snum == NULL) | (opstru.jnum == NULL) | (opstru.lfloat == NULL))
        prtusage();

    struct timeval tnow;
    unsigned tseed;
    if(opstru.uflag) {
        gettimeofday(&tnow, NULL);
        /*  use the five last digits of the current microsecond reading to generate a seed */
        tseed=(unsigned int)((tnow.tv_usec/100000.0 - tnow.tv_usec/100000)*RAND_MAX);
        srand(tseed);
    } else
        srand(SETSEED);

    char *tmpd=mktmpd();
    printf("%s\n", tmpd); 
    int ni=FPSEC*atoi(opstru.snum); // total number of images, got directly from duration in seconds.

    char nm[FNAMSZ]={0};
    char tstr[FNAMSZ]={0}; // time string

    int width=640;
    int height=480;
    float courselen=width -2*LMAR;
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create (surface);
    cairo_select_font_face (cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size (cr, FONTSZ);
    cairo_font_extents_t fe;
    cairo_text_extents_t te;
    int i;

    // race is run in advance via:
    int numreps = 2; // for now anyhow
    int mxju = atoi(opstru.jnum); /* maximum number of jumps */
    float lambd = atof(opstru.lfloat);
    vector<ev_t> *ev2=get_race(numreps, mxju, lambd, courselen);
#ifdef DBG
    prtvec(ev2, numreps);
#endif

    int t=0; // timer, 1/25 of a second or .04 of a second.
    float tp; // time proper
    unsigned ii[2]= {0,0}; //current index to check, for each participant.
    int fl[2]= {width-LMAR, width-LMAR}; //finish line
    int pos[2]= {300,340}; //current index to check, for each participant.
    int startoff=0;
    float tohere[2]={LMAR, LMAR};

    while(1) {
        sprintf(nm, "%s/ffim_%03d.png", tmpd, t);
        tp=(float)(t+1)/FPSEC; // time proper, i.e. conversion of index time (t) to proper second-fractions.
        sprintf(tstr, "%4.4f", tp);

        cairo_rectangle (cr, 0, 0, width, height); /* arg explan: topleftcorner and size of shape  */
        cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); /* setting background, probab black or grey */
        cairo_fill (cr);

        cairo_text_extents (cr, tstr, &te);
        cairo_move_to (cr, LMAR, 240);
        cairo_set_source_rgb(cr, .8, .8, .8);
        cairo_show_text (cr, tstr);

        // now the first line 
        cairo_set_source_rgba(cr, colsf[13].rgb[0], colsf[13].rgb[1], colsf[13].rgb[2], 0.95);
        // start only half way thourhg first second, and stop at finishline
        if(t>startoff) {
            for(i=0;i<2;++i) {
                if(ii[i] >= ev2[i].size()-2)
                    tohere[i] = fl[i];
                else {
                    if(tp >= ev2[i].at(ii[i]).f) { //tp has met or exceeded first float for this horse
                        tohere[i] = ev2[i].at(ii[i]).j;
                        ii[i]++; // move up to next, before doing anything else.
                    }
                }
#ifdef DBG
                printf("i %i @t %i @tf %4.4f: tohere: %4.4f\n", i, t, tp, tohere[i]);
#endif

                cairo_move_to(cr, LMAR , pos[i]); // 300 is vert pos of the line, invariable.
                // cairo_line_to(cr, JUSZ*ev2[i].at(ii[i]).j, pos[i]); // num events associated with eventjump
                cairo_line_to(cr, tohere[i], pos[i]); // num events associated with eventjump
                cairo_stroke (cr);
            }
        }

        /* Write output and clean up */
        cairo_surface_write_to_png (surface, nm);
        t++;
        if(t>ni)
            break;
        if(t>999)
            break;
    }
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    // syscalls
    // ffmpeg -framerate 25 -i ffim_%04d.png output.mp4
    char scall[SYSCMD]={0};
    sprintf(scall, "ffmpeg -loglevel quiet -framerate 25 -i %s/ffim_%%03d.png %s.mp4", tmpd, tmpd);
    system(scall);
    sprintf(scall, "rm -rf %s", tmpd);
    system(scall);

    free(tmpd);
    delete[] ev2;

    return 0;
}
