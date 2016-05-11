#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "ppm.h"
#include <string>
#include </usr/include/AL/alut.h>
extern "C" {
#include "fonts.h"
}
using namespace std;

//defined types
#define MAX_PARTICLES 5000
struct Vecs {
	float x, y, z;
};

struct Shape {
    float width, height;
    float radius;
    Vecs center;
};

struct Particle {
    Shape s;
    Vecs velocity;
};

struct Game {
    Shape box[5];
    Shape circle[2];
    Particle particle[MAX_PARTICLES];
    int n;
    int lastMousex;
    int lastMousey;
};

void init_sounds(void);
void play_music(ALuint);
void closeUpMusic(void);
bool endGame(Game *g);
bool endMenu(Game *g);
//string convertImage(string, string, string);


