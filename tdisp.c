/* creation of small images with a background, an inner rectangletrianvery worthwhile exercise, advancing step by step horizontally
 * You'll need the following to generate a vid
 * ffmpeg -framerate 25 -i ffim_%04d.png output.mp4
 */
#include <cairo/cairo.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<dirent.h> 

#define FPSEC 25 // change this to 30 if required.
#define FNAMSZ 64 // for the output file name
#define SYSCMD 128
#define FONTSZ 16
#define LMAR 20

typedef struct
{
    char nam[FNAMSZ];
    float rgb[3];
} colstru_f;

typedef enum
{
    CENCIRC /* center circle */,
    CENRECT /* center retangle */,
    LAST
} sha_t;

char *mktmpd(void)
{
    struct timeval tsecs;
    gettimeofday(&tsecs, NULL);
    char *myt=calloc(14, sizeof(char));
    strncpy(myt, "tmpdir_", 7);
    sprintf(myt+7, "%lu", tsecs.tv_usec);

    DIR *d;
    while((d = opendir(myt)) != NULL) { /* see if a directory witht the same name exists ... very low likelihood though */
        gettimeofday(&tsecs, NULL);
        sprintf(myt+7, "%lu", tsecs.tv_usec);
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
    float fsum=.0;

    for(i=0;i<n;++i) {
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

        // now the line 
        cairo_set_source_rgba(cr, colsf[15].rgb[0], colsf[15].rgb[1], colsf[15].rgb[2], 0.95);
        cairo_move_to(cr, LMAR , 300);
        fsum += finc;
        cairo_line_to(cr, fsum , 300);
        cairo_stroke (cr);


        /* Write output and clean up */
        cairo_surface_write_to_png (surface, nm);
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
