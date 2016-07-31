CC=gcc
CFLAGS=-g -Wall
SPECLIBS=-lcairo -lm
SPECINC=-I/home/nutria/mylocal/include
EXECUTABLES=binodot binind bini2 bini3 b3 b4 b5 b6 b7 b8 b9 b10 b11 b12 six b13 b14 doto

# small tiny shapes for testing on eigbirds.
smasha: smasha.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

binodot: binodot.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

# tree triangs
trndot: trndot.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

td2: td2.c
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

# Hang on, the new integrated system was not implemented properly, or I started messing?
# # Well I got over one, glitch only to be met by another. The 
# # Hmm. a lot of dabbling .. messing up the hpos
.PHONY: clean

clean:
	rm -f ${EXECUTABLES}
