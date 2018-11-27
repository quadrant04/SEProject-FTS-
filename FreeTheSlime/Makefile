# modified by: Jonathan Crawford
# author: Gordon Griesel
# Makefile for free the slime game
#
# Enter one of the following
#
# make
# make all
# make ftsgame
# make clean
#
CFLAGS = -I ./include
#LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm
LFLAGS = -lrt -lX11 -lGL

all: ftsgame

ftsgame: ftsgame.cpp log.cpp timers.cpp ryanW.cpp ivanC.cpp vananhV.cpp jonathanC.cpp
	g++ $(CFLAGS) ftsgame.cpp log.cpp timers.cpp ryanW.cpp ivanC.cpp vananhV.cpp jonathanC.cpp \
	libggfonts.a -Wall $(LFLAGS) -o ftsgame

clean:
	rm -f ftsgame
	rm -f *.o

