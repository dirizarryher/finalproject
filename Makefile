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

<<<<<<< HEAD
campusrun: campusrun.cpp davidH.cpp
	g++ $(CFLAGS) campusrun.cpp ppm.cpp log.cpp Sprite.h SOIL.h davidH.cpp libSOIL.a libggfonts.a -Wall -Wextra $(LFLAGS) -o campusrun
=======
campusrun: campusrun.cpp tyM.cpp ryanF.cpp johnB.cpp
	g++ $(CFLAGS)  campusrun.cpp tyM.cpp ryanF.cpp johnB.cpp ppm.cpp log.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o campusrun
>>>>>>> 52e43724015a1bc5b1cee00f76312d5fffb6a720
clean:
	rm -f campusrun
	rm -f *.o
