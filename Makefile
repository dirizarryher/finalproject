# cs335 hw1
# Ty Morrell CS335  
# to compile your project, type make and press enter
#CFLAGS = -I ./include
VPATH   = src include
CC      = gcc
FLAGS = -I ./include
##LIB    = ./lib/fmod/libfmodex64.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr


all: campusrun

campusrun: campusrun.cpp
	g++ $(CFLAGS) campusrun.cpp ppm.cpp log.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o campusrun
clean:
	rm -f campusrun
	rm -f *.o
