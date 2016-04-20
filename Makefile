# cs335 hw1
# Ty Morrell CS335  
# to compile your project, type make and press enter
#CFLAGS = -I ./include
VPATH   = src include
CC      = gcc
#LIBS += -framework IOKit


all: hw1

hw1: hw1.cpp
	g++ $(CFLAGS) $(LIBS) hw1.cpp -I/usr/X11R6/include -L/usr/X11R6/lib -Wall -ohw1 -lX11 -lGL -lGLU -lm 

clean:
	rm -f hw1
	rm -f *.o
