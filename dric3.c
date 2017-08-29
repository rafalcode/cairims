/*  a bouncing site race */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <cairo/cairo.h>

#define NUMSITES 16 /*  number of sites running*/
#define LENC 0.1 /* length of course */
#define BF 4

#define SETSEED 5 /*  if -DUNPREDRA is not fed in */

/*  OK; now we need cairo defines */
#define NAMBUF 32 /*  colour name length */
#define WID 640 /*  width of canvas */
#define LMAR 20 /* left margin */
#define RMAR LMAR /* right margin */
#define HEI 480 /* height of canvas */
#define HBARNUMS NUMSITES /* our horizontal bars is when the sites' tracks will be */
#define STRBF 128 /* buffer for strings */

/* only global */
const float TFONTSZ=18.0; /* Title font size */
const float FONTSZ=14.0; /* the size (number of colours) that our colorscheme has */

typedef struct /*  strcut for holding our colours, we'll be having an array of these. */
{
    char nam[NAMBUF];
    float rgb[3];
} colstru_f;

typedef struct /* our definition of a site is a struct */
{
    int sitenum; /*  number of the site */
    int numj; /* curr num of jumps: also uses as index to posarr[] */
    int jbf; /* buffer allow for jumps */
    float mxdisp; /* current maximum displacement */
    float *posarr; /* the array of position values: to be a cumulative */
} sitedef;/* our definition of a site is a struct */ 


void sitesubproc(sitedef* sites) /* the function for the site substitution process */
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
    float ura; /*  variable to hold one uniform random variable 0-1 */
    float ar[4] = {-0.886, -0.696, -1.519, -1.329};
    for(i=0;i<NUMSITES;++i) {
        while(1) { /* infinite loop to be broken out of when maxlen reaches a certain condition */
       ura= (float)rand()/RAND_MAX;
/*  armed with our ura, we can generate the Exp() */
            sites[i].posarr[sites[i].numj + 1] = sites[i].posarr[sites[i].numj] + (-1.0/ar[2])*log(1+ura); 
            sites[i].mxdisp = sites[i].posarr[sites[i].numj + 1];

            sites[i].numj++;
            /* check posarr buf sz */
            if(sites[i].numj==sites[i].jbf) {
                sites[i].jbf += BF;
                sites[i].posarr=realloc(sites[i].posarr, sites[i].jbf * sizeof(float));
            }
            /*  breaking out when condition met */
            if(sites[i].mxdisp >= LENC)
                break; /*  this site has now crossed finishing line, go to next site*/
        }
        /*  rationalise posarr array size here */
        sites[i].posarr=realloc(sites[i].posarr, sites[i].numj * sizeof(int));
    }
}

int main(int argc, char *argv[])
{
    int i, j;
    sitedef* sitearr; /*  declare */
    /* now, initialise */
    sitearr=malloc(sizeof(sitedef) * NUMSITES);
    for(i=0;i<NUMSITES;++i) {
        sitearr[i].mxdisp=0.0;
        sitearr[i].numj=0;
        sitearr[i].jbf=BF;
        sitearr[i].posarr=calloc(sizeof(float), sitearr[i].jbf);
        sitearr[i].sitenum=i+1;
    }

    /*  OK, the race takes place here: */
    sitesubproc(sitearr);

    /*  Now we can around to visualising how it was run */
    /* We want two colours for the alternating jumps of each site (from the rectnam program) but we create an array of different colours so we can choose, to get a nice scheme */
    colstru_f colsf[18] = { {"foreground", {0.866667, 0.866667, 0.866667} }, {"background", {0.066667, 0.066667, 0.066667} }, {"color0", {0.098039, 0.098039, 0.098039} }, {"color8", {0.627451, 0.321569, 0.176471} }, {"color1", {0.501961, 0.196078, 0.196078} }, {"color9", {0.596078, 0.168627, 0.168627} }, {"color2", {0.356863, 0.462745, 0.184314} }, {"color10", {0.537255, 0.721569, 0.247059} }, {"color3", {0.666667, 0.600000, 0.262745} }, {"color11", {0.937255, 0.937255, 0.376471} }, {"color4", {0.196078, 0.298039, 0.501961} }, {"color12", {0.168627, 0.309804, 0.596078} }, {"color5", {0.439216, 0.423529, 0.603922} }, {"color13", {0.509804, 0.415686, 0.694118} }, {"color6", {0.572549, 0.694118, 0.619608} }, {"color14", {0.631373, 0.803922, 0.803922} }, {"color7", {1.000000, 1.000000, 1.000000} }, {"color15", {0.866667, 0.866667, 0.866667} } };

    /*  get our canvas set up with background colour */
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, WID, HEI);
    cairo_t *cr = cairo_create (surface);
    cairo_rectangle (cr, 0, 0, WID, HEI); 
    cairo_set_source_rgba (cr, 0, 0, 0, 1);
    cairo_fill (cr);
    /*  we also want some text on the image */
    cairo_select_font_face (cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_font_extents_t fe;
    cairo_text_extents_t te;

    /*  OK, position up the sites' tracks. The HBARNUMS+1, the plus oen is for the title bar */
    int *hbars;
    hbars=calloc(HBARNUMS+1, sizeof(int)); /*  will hold midpoint of each bar */
    hbars[0] = HEI/(HBARNUMS+1)/2;
    for(i=1;i<HBARNUMS+1;++i)
        hbars[i] = hbars[i-1] + HEI/(HBARNUMS+1);

    /* title */
    cairo_set_font_size (cr, TFONTSZ);
    char tistr[]= "DNA site substitutions over time";
    cairo_text_extents (cr, tistr, &te);
    cairo_move_to (cr, WID/2 - te.x_bearing - te.width/2, hbars[0] - fe.descent + fe.height/2);
        cairo_set_source_rgb(cr, 0.5, 0.6, .782);
        cairo_show_text (cr, tistr);
    /*  and finally reset font size */
    cairo_set_font_size (cr, FONTSZ);

    /* OK, now we're going to paint up the position vectors */
    int canvfr, canvto;
    cairo_set_line_width (cr, 4);
    char *somestr;
    somestr=calloc(sizeof(char), STRBF);
    for(i=0;i<NUMSITES;++i) {
        for(j=1;j<=sitearr[i].numj;++j) {
            canvfr= LMAR + (int)(WID-RMAR-LMAR)*(sitearr[i].posarr[j-1]/LENC);
            if(j != sitearr[i].numj)
                canvto=LMAR + (int)(WID-RMAR-LMAR)*(sitearr[i].posarr[j]/LENC);
            else 
                canvto=WID-RMAR;

            if(j%2 != 0) /*  set alternate colours */ 
                cairo_set_source_rgba(cr, colsf[5].rgb[0], colsf[5].rgb[1], colsf[5].rgb[2], 0.95);
            else
                cairo_set_source_rgba(cr, colsf[11].rgb[0], colsf[11].rgb[1], colsf[11].rgb[2], 0.95);

            cairo_move_to(cr, canvfr ,hbars[i+1] +4);
            cairo_line_to(cr, canvto ,hbars[i+1] +4);
            cairo_stroke (cr);
        }
        /*  Now stick in some arbitrary text */
        sprintf(somestr, "S%i: tot %i substitutions", sitearr[i].sitenum, sitearr[i].numj-1);

        cairo_text_extents (cr, somestr, &te);
        cairo_move_to (cr, LMAR - te.x_bearing, hbars[i+1]-4 - fe.descent + fe.height/2);
        cairo_set_source_rgb(cr, 0.9, 0.8, .7);
        cairo_show_text (cr, somestr);
    }

    /* Write output and clean up */
    cairo_surface_write_to_png (surface, "dric3.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    for(i=0;i<NUMSITES;++i)
        free(sitearr[i].posarr);
    free(somestr);
    free(sitearr);
    return 0;
}
