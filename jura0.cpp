/* jump race */
#include <cairo/cairo.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
#define JUSZ 0.05

using namespace std;

typedef struct /* colstru_f, color struct */
{
    char nam[FNAMSZ];
    float rgb[3];
} colstru_f;

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
    char *jnum;
} optstruct;

int catchopts(optstruct *opstru, int oargc, char **oargv)
{
    int c;
    opterr = 0;
    while ((c = getopt (oargc, oargv, "un:l:j:")) != -1)
        switch (c) {
            case 'u': // want to see the genotypes of these SNPs
                opstru->uflag = 1;
                break;
            case 'n': // print per-sample Tech Rep resolution events
                opstru->nnum = optarg;
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

int rajuhh(int quanju) /* not equl odds, higher is rarer, of course */
{
    int i;
    int totsum=quanju*(quanju+1)/2; // typical n(n+1)/2 formula */
    float *cats=(float*)calloc(quanju-1, sizeof(float));
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

int main (int argc, char *argv[])
{
    colstru_f colsf[18] = { {"foreground", {0.866667, 0.866667, 0.866667} }, {"background", {0.066667, 0.066667, 0.066667} }, {"color0", {0.098039, 0.098039, 0.098039} }, {"color8", {0.627451, 0.321569, 0.176471} }, {"color1", {0.501961, 0.196078, 0.196078} }, {"color9", {0.596078, 0.168627, 0.168627} }, {"color2", {0.356863, 0.462745, 0.184314} }, {"color10", {0.537255, 0.721569, 0.247059} }, {"color3", {0.666667, 0.600000, 0.262745} }, {"color11", {0.937255, 0.937255, 0.376471} }, {"color4", {0.196078, 0.298039, 0.501961} }, {"color12", {0.168627, 0.309804, 0.596078} }, {"color5", {0.439216, 0.423529, 0.603922} }, {"color13", {0.509804, 0.415686, 0.694118} }, {"color6", {0.572549, 0.694118, 0.619608} }, {"color14", {0.631373, 0.803922, 0.803922} }, {"color7", {1.000000, 1.000000, 1.000000} }, {"color15", {0.866667, 0.866667, 0.866667} } };
    /* argument accounting: here were present the siddferetn shapes that are possible with this program */
    if(argc!=2) {
        printf("tdisp. a program to generate small images of certain shapes and random colours.\n");
        printf("Error. Pls supply 1 argument: 1) Number of seconds.\n");
        exit(EXIT_FAILURE);
    }

	char *tmpd=mktmpd();
    printf("%s\n", tmpd); 
    int n=FPSEC*atoi(argv[1]); // total number of images

    char nm[FNAMSZ]={0};
    char tstr[FNAMSZ]={0}; // time string
    float tf;


    int width=640;
    int height=480;
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create (surface);
    cairo_select_font_face (cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size (cr, FONTSZ);
    cairo_font_extents_t fe;
    cairo_text_extents_t te;
    int i;
    int llen= width -2*LMAR;
    float finc=(float)llen/n;
    float Zsum=LMAR;
    float fsum=LMAR;
    float fsum2=LMAR;

    i=0;
    int ii[2]= {0,0}; //current indec to check
    int cha[2]= {0,0}; // change?
    while(fsum2<llen) {
        cairo_rectangle (cr, 0, 0, width, height); /* arg explan: topleftcorner and size of shape  */
        cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); /* setting background, probab black or grey */
        cairo_fill (cr);

        sprintf(nm, "%s/ffim_%04d.png", tmpd, i);
        tf=(float)(i+1)/FPSEC;
        sprintf(tstr, "%4.4f", tf);
        cairo_text_extents (cr, tstr, &te);
        cairo_move_to (cr, LMAR, 240);
        cairo_set_source_rgb(cr, .8, .8, .8);
        cairo_show_text (cr, tstr);

        // now the first line 
        cairo_set_source_rgba(cr, colsf[13].rgb[0], colsf[13].rgb[1], colsf[13].rgb[2], 0.95);
        // start only half way thourhg first second, and stop at finishline
        int startoff=FPSEC*3;
        if(i>startoff) {
            if(fsum<llen)
                fsum += finc;
            cairo_move_to(cr, LMAR , 300); // 300 is vert pos of the line, invariable.
            cairo_line_to(cr, fsum , 300);
            cairo_stroke (cr);
        }

        // now the line 
        cairo_set_source_rgba(cr, colsf[8].rgb[0], colsf[8].rgb[1], colsf[8].rgb[2], 0.95);
        cairo_move_to(cr, LMAR , 340);
        fsum2 += 2*finc/3.;
        cairo_line_to(cr, fsum2 , 340);
        cairo_stroke (cr);

        /* Write output and clean up */
        cairo_surface_write_to_png (surface, nm);
        i++;
    }
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    // syscalls
    // ffmpeg -framerate 25 -i ffim_%04d.png output.mp4
    char scall[SYSCMD]={0};
    sprintf(scall, "ffmpeg -loglevel quiet -framerate 25 -i %s/ffim_%%04d.png %s.mp4", tmpd, tmpd);
    system(scall);
    sprintf(scall, "rm -rf %s", tmpd);
    system(scall);

    free(tmpd);

    return 0;
}
