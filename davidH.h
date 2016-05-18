#ifndef DAVID_H
#define DAVID_H
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
#include </usr/include/AL/al.h>
extern "C" {
#include "fonts.h"
}
//#include "header.h"

using namespace std;

//defined types
#define MAX_PARTICLES 5000
#define NUM_BUFFERS 50
#define NUM_SOURCES 50

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

class Sound {
    public:
        Sound();
        ~Sound();
    public:
        ALboolean volumeFlag;
        ALuint alSource;
        ALuint alBuffer;
        ALuint buffers[NUM_BUFFERS];
        ALuint sources[NUM_SOURCES];
        ALuint error;

        ALuint alSourceRunnerSprint;
        ALuint alSourceRunnerJump;
        ALuint alSourceRunnerPant;
        ALuint alSourceRunnerSlide;
        ALuint alSourceRunnerGrunt;
        ALuint alSourceRunnerDeath;
        ALuint alSourceSpear1;
        ALuint alSourceSpear2;
        ALuint alSourceSpear3;
        ALuint alSourceSpearFlurry;
        ALuint alSourceSpearLTR;
        ALuint alSourceSpearRTL;
        ALuint alSourceButton1;
        ALuint alSourceButton2;
        ALuint alSourceGameOver;
        ALuint alSourceHailToTheKing;
        ALuint alSourceOutOfGum;

        ALCcontext *Context;
        ALCdevice *Device;

        //Variables for soundboard array
        ALenum format;
        ALsizei size;
        ALsizei freq;
        ALboolean loop;
        ALvoid *data;
        ALchar *str;

        void get_flag(bool flag);
        void get_error(int err);
};

Sound sound;
float listenerPos[] = {0.0f, 0.0f, 0.0f};
float listenerVel[] = {0.0f, 0.0f, 0.0f};
float listenerOri[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
float sourcePos[] = {0.0, 0.0, 0.0};
float sourceVel[] = {0.0, 0.0, 0.0};
float sourceOri[] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};

void init_sounds(void);
void load_soundboard(void);
void play_music(ALuint, ALuint);
//void play_sound(ALuint);
//void volume(void);
void clean_music(void);
bool endGame(Game *g);
bool endMenu(Game *g);
//string convertImage(string, string, string);
string get_ALerror(int errID);


#endif
