# cs335 hw1
# Ryan Frapwelll CS335  
# to compile your project, type make and press enter
#CFLAGS = -I ./include
VPATH   = src include
CC      = gcc
FLAGS = -I ./include
##LIB    = ./lib/fmod/libfmodex64.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
AFLAGS = /usr/lib/x86_64-linux-gnu/libopenal.so \
         /usr/lib/libalut.so -o campusrun

all: campusrun

campusrun: campusrun.cpp tyM.cpp ryanF.cpp johnB.cpp davidH.cpp
	g++ $(CFLAGS)  campusrun.cpp tyM.cpp ryanF.cpp johnB.cpp davidH.cpp ppm.cpp log.cpp libggfonts.a -Wall -Wextra $(LFLAGS) $(AFLAGS) -o campusrun
clean:
	rm -f campusrun
	rm -f *.o
