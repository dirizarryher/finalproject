// David A. Hernandez II CS335
#include "davidH.h"
#include </usr/include/AL/alut.h>

Sound::Sound() 
{
    volumeFlag = false;
    error = 0;
}

Sound::~Sound() {
    delete [] buffers;
    delete [] sources;
}

void Sound::get_error(int err) 
{
    error = err;
}

void Sound::get_flag(bool flag)
{
    volumeFlag = flag;
}




string get_ALerror(int errID)
{
    if (errID == AL_NO_ERROR) {
        return "";
    }
    if (errID == AL_INVALID_NAME) {
        return "Invalid name";
    }
    else if (errID == AL_INVALID_VALUE) {
        return "Invalid value";
    }
    else if (errID == AL_INVALID_ENUM) {
        return "Invalid enum";
    }
    else if (errID == AL_INVALID_OPERATION) {
        return "Invalid operation";
    }
    else if (errID == AL_OUT_OF_MEMORY) {
        return "Not enough memory";
    }
    return "Error undefined";
}

void init_sounds(void)
{ 
    alutInit(0, NULL);
    if ((sound.error = alGetError()) != AL_NO_ERROR) {
        get_ALerror(sound.error);
        return;
    }
    
    //Clear error state
    alGetError();

    //Setup the initial positions for the listener position,
    //velocity, and orientation
    alListenerfv(AL_POSITION, listenerPos);
    alListenerfv(AL_ORIENTATION, listenerOri);
    alListenerfv(AL_VELOCITY, listenerVel);
    //alListenerf(AL_GAIN, 1.0f);

    //Create the buffers and check for errors
    alGenBuffers(NUM_BUFFERS, sound.buffers);
    if ((sound.error = alGetError()) != AL_NO_ERROR) {
        get_ALerror(sound.error);
        return;
    }
    alGetError();

    //Load the wav file
    //buffers[] holds the sound information
    sound.buffers[0] = alutCreateBufferFromFile("sounds/test.wav");
    

    //sources refers to the sound
    //Generate a source and store it into a buffer
    alGenSources(NUM_SOURCES, sound.sources);
    //alSourcei(sound.alSource, AL_BUFFER, sound.alBuffer);
    if ((sound.error = alGetError()) != AL_NO_ERROR) {
        get_ALerror(sound.error);
        return;
    }
    alGetError();

    //Set properties of the sound.
    alSourcef(sound.sources[0], AL_GAIN, 1.0f);
    alSourcef(sound.sources[0], AL_PITCH, 1.0f);
    alSourcefv(sound.sources[0], AL_POSITION, sourcePos);
    alSourcefv(sound.sources[0], AL_VELOCITY, sourceVel);
    //Source is associated with its buffer
    alSourcei(sound.sources[0], AL_BUFFER, sound.buffers[0]);
    alSourcei(sound.sources[0], AL_LOOPING, AL_FALSE);

    if ((sound.error = alGetError()) != AL_NO_ERROR) {
        printf("Error: Setting Source\n");
        get_ALerror(sound.error);
        return;
    }
/*
    for (int i = 0; i < 4; i++) {
        alSourcePlay(alSource);
        usleep(250000);
    } 
*/
}

void play_music(ALuint alSource)
{
    alSourcePlay(alSource);
}

//Creates and load an array of sounds for a soundboard
void load_soundboard(void) 
{	
    sound.buffers[1] = alutCreateBufferFromFile("sounds/runner1.wav");
    sound.buffers[2] = alutCreateBufferFromFile("sounds/runner2.wav");
    sound.buffers[3] = alutCreateBufferFromFile("sounds/runner3.wav");
    sound.buffers[4] = alutCreateBufferFromFile("sounds/runner4.wav");
    sound.buffers[5] = alutCreateBufferFromFile("sounds/runner5.wav");
    sound.buffers[6] = alutCreateBufferFromFile("sounds/runner6.wav");
    sound.buffers[7] = alutCreateBufferFromFile("sounds/spear1.wav");
    sound.buffers[8] = alutCreateBufferFromFile("sounds/spear2.wav");
    sound.buffers[9] = alutCreateBufferFromFile("sounds/spear3.wav");
    sound.buffers[10] = alutCreateBufferFromFile("sounds/spearFlurry.wav");
    sound.buffers[11] = alutCreateBufferFromFile("sounds/spearLTR.wav");
    sound.buffers[12] = alutCreateBufferFromFile("sounds/spearRTL.wav");
    sound.buffers[13] = alutCreateBufferFromFile("sounds/button1.wav");
    sound.buffers[14] = alutCreateBufferFromFile("sounds/button2.wav");
    sound.buffers[15] = alutCreateBufferFromFile("sounds/endOfGame.wav");
    sound.buffers[16] = alutCreateBufferFromFile("sounds/HailToTheKing.wav");
    sound.buffers[17] = alutCreateBufferFromFile("sounds/OutOfGum.wav");

    
    for (int i = 1; i < 17; i++) {
        alSourcef(sound.sources[i], AL_GAIN, 1.0f);
        alSourcef(sound.sources[i], AL_PITCH, 1.0f);
        alSourcefv(sound.sources[i], AL_POSITION, sourcePos);
        alSourcefv(sound.sources[i], AL_VELOCITY, sourceVel);
        alSourcei(sound.sources[i], AL_BUFFER, sound.buffers[i]);
        alSourcei(sound.sources[i], AL_LOOPING, AL_FALSE);
    }

    //Now we play the sound using:
    //alSourcePlay(source[0]);
    //Stopping sound:
    //alSourceStop(souce[0]);
    //Resetting sound to the start of the sample
    //alSourceRewind(souce[0]);
    //Pausing sound:
    //alSourcePause(souce[0]);
}

void clean_music(void) 
{
    //Sound sound;
    //Delete the soundboard source and buffer
    alDeleteSources(NUM_SOURCES, sound.sources);
    alDeleteBuffers(NUM_BUFFERS, sound.buffers);

    //Close out OPAL itself
    //Get active context
    sound.Context = alcGetCurrentContext();
    //Get device for active context
    sound.Device = alcGetContextsDevice(sound.Context);
    //Disable Context
    alcMakeContextCurrent(NULL);
    //Release Context
    alcDestroyContext(sound.Context);
    //Close Device
    alcCloseDevice(sound.Device);
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
