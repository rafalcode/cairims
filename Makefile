CC=gcc
CPP=g++
CFLAGS=-g -Wall
SPECLIBS=-lcairo -lm
SPECINC=-I/home/nutria/mylocal/include
# NOTE: beware cairomm, it can be a little devil to compile
# prob due to its being part of a bigger project
SPECINC2=`pkg-config --cflags cairomm-1.0`
SPECLIBS2=`pkg-config --libs cairomm-1.0`

EXECUTABLES=binodot binind bini2 bini3 b3 b4 b5 b6 b7 b8 b9 b10 b11 b12 six b13 b14 doto smasha2 scalin dric3 td2 rcol hsric2 scalin2 tdisp expti0 expti1 randju0 expti2 jura0 jura0_d ptinsq0 rcol2 csvrd rcol rcol3 lou0 lou1 crhmap lou2 lou3 trial0 trial1 trial2 rcol4 ptinsq0 b14a simpsq0 simpsq2 pattapp0

# small tiny shapes for testing on eigbirds. Tried to not make this output interlaced PNG files
# but cairo docs explain that this also is a toy API, no control over that.
smasha: smasha.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

# generate a quantity of small tiny shapes with random colours
# initially it was either rect of circle ... but the overall shape needs to be the same ... so now it's rect with algithly varying circle inside
smasha2: smasha2.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

binodot: binodot.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

# tree triangs
# # actually quite like this one .. it's very simple, a good building block
trndot: trndot.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

# Cairo and SVG ... don't think that just because Cairo is vector graphics that its SVG modes
# are crystal clear. In fact you need an extra h file ... cairo-svg.h
svrad0: svrad0.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

scalin: scalin.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

# can I produce a series of images showing cumulative progress of the lines?
scalin2: scalin2.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

td2: td2.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}
	./$@
	feh $@.png

simpsq0: simpsq0.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}
	./$@
	feh $@.png
simpsq2: simpsq2.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}
	./$@
	feh $@.png
# 
binind: binind.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

# 
bini2: bini2.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

# 
bini3: bini3.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

# 
b3: b3.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

# 
b4: b4.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

# 
b5: b5.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

# this is the maturest version, now working correctly, just underoptimised, that's all
b6: b6.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

# this is a slightly optimised version of b6, in that it does not use memcpy,
# but works directly on the correctly offset parts of a bigger array
# HOWEVER, you still temporary sub-arrays when you are reversing stuff
b7: b7.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

#  Started to incorporate the hbars now. This guy is working
b8: b8.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

#  Incorporated basic cairo abilities: limited to show the dots. Works.
b9: b9.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

#  The dots is one thing, the lines connecting them is another.
b10: b10.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

#  The dots is one thing, the lines connecting them is another. Started to butcher this one. It's recovered.
b11: b11.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

# 
b12: b12.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

#  Get the first six points
six: six.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

# OK, it also took me a while to get the first seven points (though I calle dthe program "six")
# And it loks as if I have to change the orientation of b12.c I need to start from the +-quadrant
# instead of the ++, and going clockwise.
#
# So now b13 should focus on changing that orientation. the graph should look the same
b13: b13.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}
# Yes, got it. Orientation is now correct

b14: b14.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}
b14a: b14a.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

# Multicolored lines with title set above
# rescuing
dric3: dric3.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

# another rescue
hsric2: hsric2.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

# Hang on, the new integrated system was not implemented properly, or I started messing?
# Hang on, the new integrated system was not implemented properly, or I started messing?
# # Well I got over one, glitch only to be met by another. The 
# # Hmm. a lot of dabbling .. messing up the hpos
#
#
# Heatmaps studies: based on rectcol.c
rcol: rcol.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}
# for plate layouts
rcol2: rcol2.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}
csvrd: csvrd.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}
rcol3: rcol3.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}
rcol4: rcolr.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}
# layout experiments
lou0: lou0.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}
lou1: lou1.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}
lou2: lou2.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}
lou3: lou3.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}
# what lou1 can do: it takes aibsolute horizontal and vert sizes for
# an object and gives you nrows of them, and ncols, all equally laid out
# with space for margins.
# How it still doesn't have text, and only copes with even nrows and even ncols
# When considering unevens, actually 3 situations arise. nrows is uneven, ncols
# is uneven, and both are uneven.
# colour heatmap
crhmap: crhmap.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}
#using  the simplified lou3.c:
crhmap2: crhmap2.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

ptinsq0: ptinsq0.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

# Just display time
tdisp: tdisp.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}
# simple exponential time variates ... hoepfully concentrating on the 1km
# # you can expect something int he range of 2 to 8 events (pretty much max, set it to 3 or 4.
expti0: expti0.c
	${CC} ${CFLAGS} -o $@ $^
expti1: expti1.cpp
	${CPP} ${CFLAGS} -o $@ $^
# learnings: a with DNA evol by Hulsenbeck, the time at which event occurs and what 
# the event transpires to be are two different things.
# if you use expone time as hop distances it's only right if time is linked to distance
# but usually, the distance is a free untied quantity, use 1, 2 or 3 hops

randju0: randju0.c
	${CC} ${CFLAGS} -o $@ $^
expti2: expti2.cpp
	${CPP} ${CFLAGS} -o $@ $^
# jump race, sticking all these together
jura0: jura0.cpp
	${CPP} ${CFLAGS} -o $@ $^ ${SPECLIBS}
jura0_d: jura0.cpp
	${CPP} ${CFLAGS} -DDBG -o $@ $^ ${SPECLIBS}
# example of cairomm which is the c++ PAI for cairo
mm0: mm0.cpp
	${CPP} ${CFLAGS} ${SPECINC2} -o $@ $^ ${SPECLIBS2}

trial0: trial0.c
	${CC} ${CFLAGS} ${SPECINC2} -o $@ $^ ${SPECLIBS2}

trial2: trial2.c
	${CC} ${CFLAGS} ${SPECINC2} -o $@ $^ ${SPECLIBS2}

pattapp0: pattapp0.c
	${CC} ${CFLAGS} ${SPECINC2} -o $@ $^ ${SPECLIBS2}

ptinsq0: ptinsq0.c
	${CC} ${CFLAGS} ${SPECINC2} -o $@ $^ ${SPECLIBS2}

.PHONY: clean

clean:
	rm -f ${EXECUTABLES}
