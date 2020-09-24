/* creation of small images with a background, an inner rectangletrianvery worthwhile exercise, advancing step by step horizontally
 * and then by powers of two vertically. In this way a binomial pattern can be drawn with cairo
 * it's critical to see how those forloop indices are built up */
#include <cairo/cairo.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum
{
    CENCIRC /* center circle */,
    CENRECT /* center retangle */,
    LAST
} sha_t;

float *randf01(int howmany)
{
    int i;
    float *r=malloc(howmany*sizeof(float));
    for(i=0;i<howmany;++i) 
        r[i]= rand()/(float)RAND_MAX;
    return r;
}

sha_t *rshat(int howmany)
{
    int i;
    sha_t *sha=malloc(howmany*sizeof(sha_t));
    for(i=0;i<howmany;++i) 
        sha[i]= (sha_t)(int)LAST*rand()/(float)RAND_MAX;
    return sha;
}

int main (int argc, char *argv[])
{
    /* argument accounting: here were present the siddferetn shapes that are possible with this program */
    if(argc!=3) {
        printf("smasha2. a program to generate small images of certain shapes and random colours.\n");
        printf("Error. Pls supply 2 arguments: 1) number of shapes to generate. 2) size in pixels (in 1D, so total pixels will be this squared).\n");
        exit(EXIT_FAILURE);
    }

    int n=atoi(argv[1]);
    int n3=3*n; // for each image we'll want 3 0-1 floats for color-setting.

    char nm[32]={0}; /* a name string */

    int width=atoi(argv[2]);
    int height=width;
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create (surface);
    int i;
    float *r=randf01(n3*2);
    float *circdiv=randf01(n); // circle divisor prep: randums 0 to 1.0
    for(i=0;i<n;++i) 
        circdiv[i] = 2*circdiv[i]+3.0; // so now divisor will vary from 3 to 6
    sha_t *rsha=rshat(n);

    /*  first we're going to set the background */
    for(i=0;i<n;++i) {
        cairo_rectangle (cr, 0, 0, width, height); /* arg explan: topleftcorner and size of shape  */
        cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); /* setting background, probab black or grey */
        cairo_fill (cr);

        /* make a simple shape .... outer one must be the same though! */
        // cairo_set_source_rgb(cr, r[3*2*i], r[3*2*i+1], r[3*2*i+2]);
        // // quick black and white
        cairo_set_source_rgb(cr, r[3*2*i], r[3*2*i], r[3*2*i]);
        cairo_rectangle (cr, width/7., height/7., 4*width/5., 4*height/5.);
        cairo_fill(cr); // when you change source colour .. you've got to cairo_fill!

        // cairo_set_source_rgb(cr, r[3*2*i+3], r[3*2*i+4], r[3*2*i+5]);
        // // quick black and white
        cairo_set_source_rgb(cr, r[3*2*i+3], r[3*2*i+3], r[3*2*i+3]);
        cairo_arc(cr, width/2., height/2., width/circdiv[i], 0, 2 * M_PI);
        cairo_fill(cr);

        // sprintf(nm, "smim_%03d.png", i);
        // // quick for B&W
        sprintf(nm, "bwim_%03d.png", i);
        /* Write output and clean up */
        cairo_surface_write_to_png (surface, nm);
    }
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    free(rsha);
    free(r);
    free(circdiv);

    return 0;
}
