# cs335 hw1
# Ty Morrell CS335  
# to compile your project, type make and press enter
CFLAGS = -I ./include


all: hw1

hw1: hw1.cpp
	g++ $(CFLAGS) hw1.cpp -Wall -ohw1 -lX11 -lGL -lGLU -lm ./libggfonts.so

clean:
	rm -f hw1
	rm -f *.o
