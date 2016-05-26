VPATH   = src include
CC      = gcc
FLAGS = -I ./include
##LIB    = ./lib/fmod/libfmodex64.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: campusrun

campusrun: campusrun.cpp tyM.cpp ryanF.cpp johnB.cpp davidH.cpp 
	g++ campusrun.cpp tyM.cpp ryanF.cpp johnB.cpp ppm.cpp davidH.cpp log.cpp libggfonts.a -Wall -Wextra $(LFLAGS) /usr/lib/x86_64-linux-gnu/libopenal.so \
/usr/lib/libalut.so -o campusrun
clean:
	rm -f campusrun
	rm -f *.o
