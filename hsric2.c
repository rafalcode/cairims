/*  a bouncing horse race: 
 *  BEWARE this uses simple random
 *  numbers and not the exponential waiting time */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <cairo/cairo.h>

#define NUMHO 6 /*  number of horses running*/
#define LENC 100 /* length of course */
#define JSZ 20 /*  jump size */
#define BF 4

#define SETSEED 5 /*  if -DUNPREDRA is not fed in */

/*  OK; now we need cairo defines */
#define NAMBUF 32 /*  colour name length */
#define WID 640 /*  width of canvas */
#define LMAR 20 /* left margin */
#define RMAR LMAR /* right margin */
#define HEI 480 /* height of canvas */
#define HBARNUMS NUMHO /* our horizontal bars is when the horses' tracks will be */
#define STRBF 128 /* buffer for strings */
#define LTHI 16 /* line thickness */

/* only global */
const float FONTSZ=14.0; /* the size (number of colours) that our colorscheme has */

typedef struct {
    char nam[NAMBUF];
    float rgb[3];
} colstru_f;

typedef struct /* our definition of a horse is a struct */
{
    int honum; /*  number of the horse */
    int numj; /* curr num of jumps: also uses as index to posarr[] */
    int jbf; /* buffer allow for jumps */
    int mxdisp; /* current maximum displacement */
    int *posarr; /* the array of position values: to be a cumulative */
} hodef;/* our definition of a horse is a struct */ 


void hsru(hodef* hossies)
{
    int i;
#ifdef UNPREDRA
    struct timeval tnow;
    gettimeofday(&tnow, NULL);
    /*  use the five last digits of the current microsecond reading to generate a seed */
    unsigned int tseed=(unsigned int)((tnow.tv_usec/100000.0 - tnow.tv_usec/100000)*RAND_MAX);
    printf("ts=%7.5f\n", (tnow.tv_usec/100000.0 - tnow.tv_usec/100000));
    printf("RAND_MAX: %u\n", RAND_MAX); 
    printf("time-seed came out as: %u\n", tseed); 
    srand(tseed);
#else
    srand(SETSEED);
#endif

    for(i=0;i<NUMHO;++i) {
        while(1) { /* infinite loop to be broken out of when maxlen reaches a certain condition */
            hossies[i].posarr[hossies[i].numj + 1] = hossies[i].posarr[hossies[i].numj] + rand()%JSZ;
            hossies[i].mxdisp = hossies[i].posarr[hossies[i].numj + 1];

            hossies[i].numj++;
            /* check posarr buf sz */
            if(hossies[i].numj==hossies[i].jbf) {
                hossies[i].jbf += BF;
                hossies[i].posarr=realloc(hossies[i].posarr, hossies[i].jbf * sizeof(int));
            }
            /*  breaking out when condition met */
            if(hossies[i].mxdisp >= LENC)
                break; /*  this horse has now crossed finishing line, go to next horse*/
        }
        /*  rationalise posarr array size here */
        hossies[i].posarr=realloc(hossies[i].posarr, hossies[i].numj * sizeof(int));
    }
}

int main(int argc, char *argv[])
{
    int i, j;
    hodef* hosdarr;
    hosdarr=malloc(sizeof(hodef) * NUMHO);
    for(i=0;i<NUMHO;++i) {
        hosdarr[i].mxdisp=0;
        hosdarr[i].numj=0;
        hosdarr[i].jbf=BF;
        hosdarr[i].posarr=calloc(sizeof(int), hosdarr[i].jbf);
        hosdarr[i].honum=i;
    }

    /*  OK, the race takes lace here: */
    hsru(hosdarr);
    /*  Now we can around to visualising */
    /* We want two colours for the alternating jumps of each horse (from the rectnam program) but we create an array of different colours so we can choose, to get a nice scheme */
    colstru_f colsf[18] = { {"foreground", {0.866667, 0.866667, 0.866667} }, {"background", {0.066667, 0.066667, 0.066667} }, {"color0", {0.098039, 0.098039, 0.098039} }, {"color8", {0.627451, 0.321569, 0.176471} }, {"color1", {0.501961, 0.196078, 0.196078} }, {"color9", {0.596078, 0.168627, 0.168627} }, {"color2", {0.356863, 0.462745, 0.184314} }, {"color10", {0.537255, 0.721569, 0.247059} }, {"color3", {0.666667, 0.600000, 0.262745} }, {"color11", {0.937255, 0.937255, 0.376471} }, {"color4", {0.196078, 0.298039, 0.501961} }, {"color12", {0.168627, 0.309804, 0.596078} }, {"color5", {0.439216, 0.423529, 0.603922} }, {"color13", {0.509804, 0.415686, 0.694118} }, {"color6", {0.572549, 0.694118, 0.619608} }, {"color14", {0.631373, 0.803922, 0.803922} }, {"color7", {1.000000, 1.000000, 1.000000} }, {"color15", {0.866667, 0.866667, 0.866667} } };

    /*  get our canvas set up with background colour */
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, WID, HEI);
    cairo_t *cr = cairo_create (surface);
    cairo_rectangle (cr, 0, 0, WID, HEI); 
    cairo_set_source_rgba (cr, 0, 0, 0, 1);
    cairo_fill (cr);
    /*  we also want some text on the image */
    cairo_select_font_face (cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size (cr, FONTSZ);
    cairo_font_extents_t fe;
    cairo_text_extents_t te;

    /*  OK, position up the horses' tracks */
    int *hbars;
    hbars=calloc(HBARNUMS, sizeof(int)); /*  will hold midpoint of each bar */
    hbars[0] = HEI/HBARNUMS/2;
    for(i=1;i<HBARNUMS;++i)
        hbars[i] = hbars[i-1] + HEI/HBARNUMS;

    /* OK, now we're going to paint up the position vectors */
    int canvfr, canvto;
    cairo_set_line_width (cr, LTHI);
    // char somestr[]="testtextsuavemente";
    char *somestr;
    somestr=calloc(sizeof(char), STRBF);
    for(i=0;i<NUMHO;++i) {
        for(j=1;j<=hosdarr[i].numj;++j) {
            canvfr= LMAR + (int)(WID-RMAR-LMAR)*((float)hosdarr[i].posarr[j-1]/LENC);
            if(j != hosdarr[i].numj)
                canvto=LMAR + (int)(WID-RMAR-LMAR)*((float)hosdarr[i].posarr[j]/LENC);
            else 
                canvto=WID-RMAR;

            if(j%2 != 0) /*  set alternate colours */ 
                cairo_set_source_rgba(cr, colsf[7].rgb[0], colsf[7].rgb[1], colsf[7].rgb[2], 0.95);
            else
                cairo_set_source_rgba(cr, colsf[15].rgb[0], colsf[15].rgb[1], colsf[15].rgb[2], 0.95);

            cairo_move_to(cr, canvfr ,hbars[i] +4);
            cairo_line_to(cr, canvto ,hbars[i] +4);
            cairo_stroke (cr);
        }
            /*  Now stick in some arbitrary text */
        sprintf(somestr, "H%i: tot %i strides; pos': ", hosdarr[i].honum+1, hosdarr[i].numj);
        /* print the array of positional values values too? */

            cairo_text_extents (cr, somestr, &te);
            // horiz pos, the integer substracting from hbars is the most influential
            cairo_move_to (cr, LMAR - te.x_bearing, hbars[i]-12 - fe.descent + fe.height);
            cairo_set_source_rgb(cr, .8, .8, .8);
            cairo_show_text (cr, somestr);
    }

    /* Write output and clean up */
    cairo_surface_write_to_png (surface, "hsric2.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    for(i=0;i<NUMHO;++i)
        free(hosdarr[i].posarr);
    free(somestr);
    free(hosdarr);
    return 0;
}
