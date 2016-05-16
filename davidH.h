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
#define NUM_BUFFERS 32
#define NUM_SOURCES 32

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
    ALuint Sound;
};

struct Game {
    Shape box[5];
    Shape circle[2];
    Particle particle[MAX_PARTICLES];
    int n;
    int lastMousex;
    int lastMousey;
};


ALCcontext *context;
ALCdevice *device;
ALuint error;
float vec[6] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};

//Variables for soundboard array
ALenum format;
ALsizei size;
ALsizei freq;
ALboolean loop;
ALvoid* data;
ALfloat listenerPos[] = {0.0, 0.0, 0.0};
ALfloat listenerVel[] = {0.0, 0.0, 0.0};
ALfloat listenerOri[] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};
ALfloat sourcePos[] = {0.0, 0.0, 0.0};
ALfloat sourceVel[] = {0.0, 0.0, 0.0};
ALfloat sourceOri[] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};

ALuint buffers[NUM_BUFFERS];
ALuint *source = new ALuint(source[0]);

void init_sounds(void);
void load_soundboard(void);
void play_music(ALuint, ALuint);
void close_music(void);
bool endGame(Game *g);
bool endMenu(Game *g);
//string convertImage(string, string, string);
string get_ALerror(int errID);
