# cs335 hw1
# Ryan Frapwelll CS335  
# to compile your project, type make and press enter
#CFLAGS = -I ./include
VPATH   = src include
CC      = gcc
FLAGS = -I ./include
##LIB    = ./lib/fmod/libfmodex64.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr


all: campusrun

campusrun: campusrun.cpp davidH.cpp
	g++ $(CFLAGS) campusrun.cpp ppm.cpp log.cpp Sprite.h SOIL.h davidH.cpp libSOIL.a libggfonts.a -Wall -Wextra $(LFLAGS) -o campusrun
clean:
	rm -f campusrun
	rm -f *.o
