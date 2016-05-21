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
#include <sys/stat.h>
#include <fcntl.h>
#include </usr/include/AL/alut.h>
#include </usr/include/AL/al.h>
extern "C" {
#include "fonts.h"
}

using namespace std;

//defined types
#define MAX_PARTICLES 5000
#define NUM_BUFFERS 50
#define NUM_SOURCES 50

class Sound {
    public:
        Sound();
        
		ALboolean volumeFlag;
        ALuint buffers[NUM_BUFFERS];
        ALuint sources[NUM_SOURCES];
        ALuint error;

        //Variables for soundboard array
        ALenum format;
        ALsizei size;
        ALsizei freq;
        ALboolean loop;
        ALvoid *data;
        ALchar *str;

        void get_flag(bool flag);
        void get_error(int err);
        void get_listener(float, float, float);
        void get_source(float, float, float);
};

extern Sound sound;
void init_sounds(void);
void play_sound(ALuint);
//void volume(void);
void clean_music(void);
//bool endGame(Game *g);
//bool endMenu(Game *g);
//string convertImage(string, string, string);
string get_ALerror(int errID);

#endif
