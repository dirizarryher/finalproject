// David A. Hernandez II CS335
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include </usr/include/AL/alut.h>
#include </usr/include/AL/al.h>
extern "C" {
#include "fonts.h"
}
using namespace std;

#define NUM_BUFFERS 20
#define NUM_SOURCES 20
ALfloat listenerPos[3] = {0.0f, 0.0f, 0.0f};
ALfloat listenerVel[3] = {0.0f, 0.0f, 0.0f};
float listenerOri[6] = {0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};
ALfloat sourcePos[3] = {0.0f, 0.0f, 0.0f};
ALfloat sourceVel[3] = {0.0f, 0.0f, 0.0f};
ALfloat sourceOri[6] = {0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};
ALuint alSource[18];
ALuint alBuffer[18];
ALboolean volumeFlag;
ALenum error;
string get_ALerror(int errID);
void init_sounds(void);
void play_music(void);
void play_sound(void);
void clean_sounds(void);

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

	if ((error = alGetError()) != AL_NO_ERROR) {
		get_ALerror(error);
		fprintf(stderr, "ALUT Error: %s\n", alutGetErrorString(alutGetError()));
		return;
	}
	//Clear error state
	alGetError();


	//Setup the initial positions for the listener position,
	//velocity, and orientation
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerf(AL_GAIN, 1.0f);

	//buffers[] holds the sound information
	//This is the main menu music
	alBuffer[0] = alutCreateBufferFromFile("./sounds/test.wav\0");
	//Running Sound
	alBuffer[1] = alutCreateBufferFromFile("sounds/runner1.wav\0");
	//Heavy Breathing Sound
	alBuffer[2] = alutCreateBufferFromFile("sounds/runner2.wav\0");
	//Sliding Sound
	alBuffer[3] = alutCreateBufferFromFile("sounds/runner3.wav\0");
	//Sound for dead runner (screaming)
	alBuffer[4] = alutCreateBufferFromFile("sounds/runner4.wav\0");
	//Runner in Pain
	alBuffer[5] = alutCreateBufferFromFile("sounds/runner5.wav\0");
	//Runner Jumping
	alBuffer[6] = alutCreateBufferFromFile("sounds/runner6.wav\0");

	//Variations of spears thrown
	alBuffer[7] = alutCreateBufferFromFile("sounds/spear1.wav\0");
	alBuffer[8] = alutCreateBufferFromFile("sounds/spear2.wav\0");
	alBuffer[9] = alutCreateBufferFromFile("sounds/spear3.wav\0");
	//Sound of multiple spears thrown
	alBuffer[10] = alutCreateBufferFromFile("sounds/spearFlurry.wav\0");
	//Spear thrown from Left to Right
	alBuffer[11] = alutCreateBufferFromFile("sounds/spearLTR.wav\0");
	//Spear thrown from Right to Left
	alBuffer[12] = alutCreateBufferFromFile("sounds/spearRTL.wav\0");

	//Enter button
	alBuffer[13] = alutCreateBufferFromFile("sounds/button1.wav\0");
	//Back button
	alBuffer[14] = alutCreateBufferFromFile("sounds/button2.wav\0");

	//Game Over Sound
	alBuffer[15] = alutCreateBufferFromFile("sounds/endOfGame.wav\0");
	//Hail to the King Baby!
	alBuffer[16] = alutCreateBufferFromFile("./sounds/HailToTheKing.wav\0");
	//Homage to DOOM
	alBuffer[17] = alutCreateBufferFromFile("sounds/OutOfGum.wav\0");


	//Sources[] refer to the sound
	//Generate a source and store it into their respective buffers
	//alGenSources(NUM_SOURCES, alSource);
	for (int i = 0; i < 18; i++) {
		alGenSources(1, &alSource[i]);
		alSourcei(alSource[i], AL_BUFFER, alBuffer[i]);

		//Set properties for each sound
		alSourcef(alSource[i], AL_GAIN, 1.0f);
		alSourcef(alSource[i], AL_PITCH, 1.0f);
		alSourcefv(alSource[i], AL_POSITION, sourcePos);
		alSourcefv(alSource[i], AL_VELOCITY, sourceVel);
		//Set a looping sound for Main Menu music
//if (i == 0) {
	//		alSourcei(alSource[0], AL_LOOPING, AL_TRUE);
		//}
		//else
		alSourcei(alSource[i], AL_LOOPING, AL_FALSE);
		printf("Loading source[%d]\n", i);
	}

	if ((error = alGetError()) != AL_NO_ERROR) {
		get_ALerror(error);
		return;
	} else {
		printf("Loading soundboard\n");
	}
	if ((error = alGetError()) != AL_NO_ERROR) {
		printf("Error: Setting Source\n");
		get_ALerror(error);
		return;
	} else {
		printf("Sources have been set\n");
	}
	alGetError();

	
	//Now we play the sound using:
	//alSourcePlay(source[0]);
	//Stopping sound:
	//alSourceStop(souce[0]);
	//Resetting sound to the start of the sample
	//alSourceRewind(souce[0]);
	//Pausing sound:
	//alSourcePause(souce[0]);

}

void play_music(void)
{
    alSourcePlay(alSource[0]);
    printf("Playing sound %i", alSource[0]);
}

void play_sound(void) 
{
	alSourcePlay(alSource[0]);
	printf("Playing sound %i", alSource[0]);
}

void clean_sounds(void) 
{
	//Sound sound;
	//Delete the soundboard source and buffer
	for (int i = 0; i < 18; i++) {
		alDeleteSources(NUM_SOURCES, &alSource[i]);
		alDeleteBuffers(NUM_BUFFERS, &alBuffer[i]);
		printf("Deleting source[%d]\n", i);
	}

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
