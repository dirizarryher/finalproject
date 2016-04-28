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
campusrun: campusrun.cpp tyM.cpp ryanf.cpp
	g++ $(CFLAGS)  campusrun.cpp header.h tyM.cpp ryanf.cpp ppm.cpp log.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o campusrun
=======
campusrun: campusrun.cpp tyM.cpp ryanF.cpp
	g++ $(CFLAGS)  campusrun.cpp tyM.cpp ryanF.cpp ppm.cpp log.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o campusrun
>>>>>>> 29d57a8a6f13b4ebe91ec736e9be29be04263ba9
clean:
	rm -f campusrun
	rm -f *.o
