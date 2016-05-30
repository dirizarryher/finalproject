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
float listenerOri[6] = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};
ALfloat sourcePos[3] = {0.0f, 0.0f, 0.0f};
ALfloat sourceVel[3] = {0.0f, 0.0f, 0.0f};
ALfloat sourceOri[6] = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};
ALuint alSource[NUM_BUFFERS];
ALuint alBuffer[NUM_SOURCES];
ALboolean volumeFlag;
ALenum error;
extern bool play;
extern bool king;
extern int jump;
extern int slide;
extern int dead;


string get_ALerror(ALenum errID)
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
		printf("%s\n", get_ALerror(error).c_str());
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
	alBuffer[16] = alutCreateBufferFromFile("sounds/Hail.wav\0");
	//Homage to DOOM
	alBuffer[17] = alutCreateBufferFromFile("sounds/Gum.wav\0");

	//Generate a source and store it into their respective buffers
	//alGenSources(NUM_SOURCES, alSource);
	for (int i = 0; i < NUM_SOURCES; i++) {
		alGenSources(1, &alSource[i]);
		alSourcei(alSource[i], AL_BUFFER, alBuffer[i]);

		//Set properties for each sound
		alSourcef(alSource[i], AL_GAIN, 1.0f);
		alSourcef(alSource[i], AL_PITCH, 1.0f);
		alSourcefv(alSource[i], AL_POSITION, sourcePos);
		alSourcefv(alSource[i], AL_VELOCITY, sourceVel);
		alSourcei(alSource[i], AL_LOOPING, AL_FALSE);
		if ((error = alGetError()) != AL_NO_ERROR) {
			printf("%s\n", get_ALerror(error).c_str());
			fprintf(stderr, "ALUT Error: %s\n", alutGetErrorString(alutGetError()));
			return;
		} else {
			//printf("Loading source[%d]\n", i);
		}
		alGetError();
	};

	if ((error = alGetError()) != AL_NO_ERROR) {
		printf("%s\n", get_ALerror(error).c_str());
		fprintf(stderr, "ALUT Error: %s\n", alutGetErrorString(alutGetError()));
		return;
	} else {
		printf("Loading soundboard\n");
	}
	alGetError();

	if ((error = alGetError()) != AL_NO_ERROR) {
		printf("Error: Setting Source\n");
		printf("%s\n", get_ALerror(error).c_str());
		fprintf(stderr, "ALUT Error: %s\n", alutGetErrorString(alutGetError()));
		return;
	} else {
		printf("Sources have been set\n");
	}
	alGetError();

	//Running attributes
	alSourcef(alSource[1], AL_GAIN, 0.5f);
	alSourcef(alSource[1], AL_PITCH, 2.0f);
	alSourcei(alSource[1], AL_LOOPING, AL_FALSE);

	//Heavy Breathing attributes
	alSourcef(alSource[2], AL_GAIN, 0.6f);
	alSourcef(alSource[2], AL_PITCH, 0.7f);
	alSourcei(alSource[2], AL_LOOPING, AL_FALSE);

	//Dead runner attributes
	alSourcef(alSource[4], AL_GAIN, 0.9f);
	alSourcef(alSource[4], AL_PITCH, 1.5f);

	//Pain sound attributes
	alSourcef(alSource[5], AL_GAIN, 0.2f);
	alSourcef(alSource[5], AL_PITCH, 1.3f);

	//Jump sound attributes
	alSourcef(alSource[6], AL_GAIN, 0.9f);
	alSourcef(alSource[6], AL_PITCH, 1.3f);

	//endGame attibutes
	alSourcef(alSource[15], AL_GAIN, 1.5f);
	alSourcef(alSource[15], AL_PITCH, 1.0f);

	//HTTK attributes
	alSourcef(alSource[16], AL_GAIN, 1.0f);
	alSourcef(alSource[16], AL_PITCH, 1.0f);

	//OOG attributes
	alSourcef(alSource[17], AL_GAIN, 1.25f);
	alSourcef(alSource[17], AL_PITCH, 1.0f);

}

//Change 1 (Run), 2(Breathing), 4(Dead), 5(Pain), 6(Jump)
//Modify 15(end), 16(king), 17(gum)
//Slide 3
//Spears 7,8,9, 10(flurry), 11(LTR), 12(RTL)
//Buttons 13, 14

void play_music(void)
{
	alSourcePlay(alSource[0]);
	//printf("Playing sound %i", alSource[0]);
	play = !play;
}

void play_jumpsound(void) 
{
	if (!dead && play){
		alSourcePlay(alSource[6]);
		//printf("Playing sound %i", alSource[6]);
		play = !play;
	}
}

void play_slide(void) 
{
	if (!dead && play){
		alSourcePlay(alSource[3]);
		printf("Playing sound %d\n", alSource[3]);
		play = !play;
	}
	//printf("Playing sound %i", alSource[3]);
}

void play_dead(void) 
{
	alSourcePlay(alSource[4]);
	//printf("Playing sound %i", alSource[4]);
}

void play_end(void)
{
	alSourcePlay(alSource[15]);
	//printf("Playing sound %i", alSource[15]);
}

void play_king(void)
{
	if ((!dead && king)){
		alSourcePlay(alSource[15]);
		printf("Playing sound %d\n", alSource[16]);
		king = !king;
	}
}

/* void alDopplerFactor (
 * 	ALfloat value
 * 	);
 * value = the Dopller scale value to set
 * The default Doppler factor value is 1.0
 */

void clean_sounds(void) 
{
	//Sound sound;
	//Delete the soundboard source and buffer
	for (int i = 0; i < NUM_SOURCES; i++) {
		alDeleteSources(1, &alSource[i]);
		alDeleteBuffers(1, &alBuffer[i]);
		//fprintf("Deleting source[%d]\n", i);
		if ((error = alGetError()) != AL_NO_ERROR) {
			//printf("Error: Deleting Sounds\n");
			printf("%s\n", get_ALerror(error).c_str());
			fprintf(stderr, "ALUT Error: %s\n", alutGetErrorString(alutGetError()));
			return;
		}
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

//Converts an image with any file extension imported into 
//images/runner/ directory into a ppm image
string convertImage(string filename, string path, string filetype)
{
	<<<<<<< HEAD
		string oldfile = path + filename + filetype;
	string newfile = path + filename + ".ppm";

	//converting sprite.png to sprite.ppm
	string newstuff =  "convert " + oldfile + " " + newfile;
	filename = system(newstuff.data());
	return filename;
}

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
