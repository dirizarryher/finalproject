// David A. Hernandez II CS335
#include "davidH.h"
#include </usr/include/AL/alut.h>

void init_sounds(void)
{
    alutInit(0, NULL);
    if (alGetError() != AL_NO_ERROR) {
        printf("Error: alutInit()\n");
        return;
    }

    //Clear error state
    alGetError();
    
    //Setup Listener
    float vec[6] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListenerfv(AL_ORIENTATION, vec);
    alListenerf(AL_GAIN, 1.0f);

    //Buffer holds the sound information
    ALuint alBuffer;
    alBuffer = alutCreateBufferFromFile("./test.wav");

    //Source refers to the sound
    ALuint alSource;
    //Generate a source and store it into a buffer
    alGenSources(1, &alSource);
    alSourcei(alSource, AL_BUFFER, alBuffer);

    //Set volume and pitch to normal, no looping of sound
    alSourcef(alSource, AL_GAIN, 1.0f);
    alSourcef(alSource, AL_PITCH, 1.0f);
    alSourcei(alSource, AL_LOOPING, AL_FALSE);
    if (alGetError() != AL_NO_ERROR) {
        printf("Error: Setting Source\n");
        return;
    }
    
    for (int i = 0; i < 4; i++) {
        alSourcePlay(alSource);
        usleep(250000);
    }
}

void play_music(ALuint alSource)
{
    alSourcePlay(alSource);
}

void cleanUpMusic(ALuint &alBuffer, ALuint alSource) 
{
    //First, delete the source
    alDeleteSources(1, &alSource);
    //Delete the buffer
    alDeleteBuffers(1, &alBuffer);

    //Close out OPAL itself
    //Get active context
    ALCcontext *Context = alcGetCurrentContext();
    //Get device for active context
    ALCdevice *Device = alcGetContextsDevice(Context);
    //Disable Context
    alcMakeContextCurrent(NULL);
    //Release Context
    alcDestroyContext(Context);
    //Close Device
    alcCloseDevice(Device);
}
/*
//Converts an image with any file extension imported into 
//images/runner/ directory into a ppm image
string convertImage(string filename, string path, string filetype)
{
    string oldfile = path + filename + filetype;
    string newfile = path + filename + ".ppm";

    //convert ./images/runner/runner_sheet2.ppm 
    //./images/runner/runner_sheet2.png
    string newstuff =  "convert " + oldfile + " " + newfile;
    filename = system(newstuff.data());
    return filename;
}
*/
//Game Over Menu if runner dies
/*void endMenu(Game *g)
{
    int yellow = 0x00ffff00;
    int red = 0x00ff0000;

    Rect r;
    r.bot = yres - 300;
    r.left = 600;
    ggprint16(&r, 50, red, "GAME OVER");
    ggprint16(&r, 50, red, "You died!");
    ggprint16(&r, 50, yellow, "Your Score: %i", g->score);
    ggprint16(&r, 50, yellow, "Your Time: %i seconds", g->gameTimer); 

    r.bot = yres - 600;
    ggprint16(&r, 50, yellow, "Press ESC to Exit");
}
*/
