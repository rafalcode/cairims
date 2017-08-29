/* scalin: scaling line */
#include <cairo/cairo.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h> // required for optopt, opterr and optarg.

/* OFFSETS to allow for frame */
#define OFFW 15
#define OFFH 15

#define HBARNUMS 8

typedef unsigned char boole;

typedef struct  /* opt_t, a struct for the options */
{
	int h, w; /* height and width of image */
} opt_t;

typedef struct /* two integers: (x) hor-position and (y) vert position */
{
	int x, y;
} pos;

void prtusage(void)
{
	printf("scalin: an image geenrator using cairo: please supply values for the following options: \n"); 
	printf("-w: (integer) the width of the image\n");
	printf("-h: (integer) the height of the image\n");
	return;
}

int catchopts(opt_t *opts, int oargc, char **oargv)
{
	int i, c;
	opterr = 0;

	while ((c = getopt (oargc, oargv, "h:w:")) != -1) {
		switch (c) {
			case 'h':
				opts->h = atoi(optarg);
				break;
			case 'w':
				opts->w = atoi(optarg);
				break;
			case '?':
				fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				return 1;
			default:
				fprintf (stderr, "Wrong arguments. Please launch without arguments to see help file.\n");
				exit(EXIT_FAILURE);
		}
	}
	boole nonopt=0;
	for(i=optind;i<oargc;++i) {
		printf("Error: \"%s\"?: Arguments only accepted if specified with \"-<letter>\" option argument.\n", oargv[i]);
		nonopt=1;
	}
	if(nonopt) {
		printf("\nArgument errors: usage instructions are as follows:\n\n"); 
		prtusage();
	}
	return 0;
}

int main (int argc, char *argv[])
{
	if(argc == 1) {
		prtusage();
		exit(EXIT_FAILURE);
	}
	opt_t opts={0};
	catchopts(&opts, argc, argv);
	int i, width=opts.w, height=opts.h;

	cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
	cairo_t *cr = cairo_create (surface);

	/*  first we're going to set the background */
	cairo_rectangle (cr, 0, 0, width, height); /* arg explan: topleftcorner and size of shape  */
	cairo_set_source_rgba (cr, 0, 0, 0, 0.95); /*  final number is alpha, 1.0 is opaque */
	cairo_fill (cr);

	/* We want a frame ... mostly this will have a limited set of values */
	int ws2=OFFW /* start point for width */, hs2=OFFH /* ditto height */, we2=width-OFFW /* end point for width */, he2=height-OFFH; /* ditto height */
	int w2=width-ws2-OFFW, h2=height-hs2-OFFH; /* these are the new widths and heights */
	cairo_rectangle (cr, ws2, hs2, w2, h2); /* arg explan: topleftcorner and size of shape  */
	cairo_set_source_rgba (cr, 0.3, 0.65, 0, 0.95); /*  final number is alpha, 1.0 is opaque */
	cairo_fill (cr);

	/* vertical divider how ar ewe going to section off the screen vertically */
	float xbar=(float)w2/HBARNUMS; /* answer is: equally */
	float xb1=xbar; /* this is the first hor bar ... usually for labelling y-axis */
	float vbar=(float)h2/HBARNUMS; /* answer is: equally */
	float tb1=hs2+vbar; /* top bar, usually for title */
	float bb1=he2; /* bottom bar, usually for x-axis title */
	pos ori, *lb=calloc(HBARNUMS, sizeof(pos)); /* ori: origin, lb, last bar, */
	/* arrange array of positions */
	ori.x=ws2+xb1;
	ori.y=hs2+h2/2.;

	lb[0].x= w2-xbar/2.;
	lb[0].y = vbar/2.;
	for(i=1;i<HBARNUMS;i++) {
		lb[i].x= w2-xbar/2.;
		lb[i].y = lb[i-1].y + vbar;
	}

	cairo_set_source_rgba(cr, 0.65, 0.8, 0.45, 0.6);
	for(i=0;i<HBARNUMS;i++){
		cairo_arc(cr, lb[i].x, lb[i].y, 1, 0, 2 * M_PI);
		cairo_fill(cr);

		cairo_move_to(cr,ori.x,ori.y);
		cairo_line_to(cr,lb[i].x, lb[i].y);
		cairo_stroke(cr);
	}

	/* Write output and clean up */
	cairo_surface_write_to_png (surface, "scalin.png");
	cairo_destroy (cr);
	cairo_surface_destroy (surface);
	free(lb);

	return 0;
}
