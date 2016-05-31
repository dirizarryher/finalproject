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

#define NUM_BUFFERS 25
#define NUM_SOURCES 25
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
extern int jump, slide, dead;
extern bool play, king, spear, button1, button2, alien, confirmed;

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
	alBuffer[0] = alutCreateBufferFromFile("./sounds/ambiance.wav\0");
	//Running Sound
	alBuffer[1] = alutCreateBufferFromFile("sounds/runner1.wav\0");
	//Heavy Breathing Sound
	alBuffer[2] = alutCreateBufferFromFile("sounds/runner2.wav\0");
	//Sliding Sound
	alBuffer[3] = alutCreateBufferFromFile("sounds/runner3.wav\0");
	//Sound for dead runner (screaming)
	alBuffer[4] = alutCreateBufferFromFile("sounds/runner4.wav\0");
	//Runner Jumping
	alBuffer[5] = alutCreateBufferFromFile("sounds/runner5.wav\0");
	//Variations for speedboost sound
	alBuffer[6] = alutCreateBufferFromFile("sounds/boost.wav\0");
	alBuffer[7] = alutCreateBufferFromFile("sounds/Powerup.wav\0");
	
	//Variations of spears thrown
	alBuffer[8] = alutCreateBufferFromFile("sounds/spear1.wav\0");
	alBuffer[9] = alutCreateBufferFromFile("sounds/spear2.wav\0");
	alBuffer[10] = alutCreateBufferFromFile("sounds/spear3.wav\0");
	//Sound of multiple spears Right to Left
	alBuffer[11] = alutCreateBufferFromFile("sounds/spearRTL.wav\0");

	//Variation of Monster sounds
	alBuffer[12] = alutCreateBufferFromFile("sounds/MonsterGrowl.wav\0");
	alBuffer[13] = alutCreateBufferFromFile("sounds/MonsterLaugh.wav\0");
	alBuffer[14] = alutCreateBufferFromFile("sounds/mokay.wav\0");

	//Enter button
	alBuffer[15] = alutCreateBufferFromFile("sounds/button1.wav\0");
	//Back button
	alBuffer[16] = alutCreateBufferFromFile("sounds/button2.wav\0");
	//Game Over Sound
	alBuffer[17] = alutCreateBufferFromFile("sounds/endOfGame.wav\0");
	//John Cena
	alBuffer[18] = alutCreateBufferFromFile("sounds/JohnCena.wav\0");
	//AlienEnter
	alBuffer[19] = alutCreateBufferFromFile("sounds/Alien.wav\0");
	//AlienExit
	alBuffer[20] = alutCreateBufferFromFile("sounds/Alien2.wav\0");

	//ILLUMINATI
	alBuffer[21] = alutCreateBufferFromFile("sounds/Illuminati.wav\0");
	//Bat Sound
	alBuffer[22] = alutCreateBufferFromFile("sounds/bat.wav\0");
		
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
		
		//Ambiance attributes
		alSourcef(alSource[0], AL_GAIN, 0.6f);
		alSourcef(alSource[0], AL_PITCH, 1.0f);
		alSourcei(alSource[0], AL_LOOPING, AL_TRUE);
		
		//Running attributes
		alSourcef(alSource[1], AL_GAIN, 0.5f);
		alSourcef(alSource[1], AL_PITCH, 2.0f);
		alSourcei(alSource[1], AL_LOOPING, AL_FALSE);
		
		//Heavy Breathing attributes
		alSourcef(alSource[2], AL_GAIN, 0.6f);
		alSourcef(alSource[2], AL_PITCH, 0.7f);
		alSourcei(alSource[2], AL_LOOPING, AL_FALSE);
		
		//Sliding attributes
		alSourcef(alSource[3], AL_GAIN, 0.6f);
		alSourcef(alSource[3], AL_PITCH, 0.7f);
		alSourcei(alSource[3], AL_LOOPING, AL_FALSE);

		//Dead runner attributes
		alSourcef(alSource[4], AL_GAIN, 2.0f);
		alSourcef(alSource[4], AL_PITCH, 1.5f);
			
		//Jump sound attributes
		alSourcef(alSource[5], AL_GAIN, 0.1f);
		alSourcef(alSource[5], AL_PITCH, 1.3f);
		
		//Speedboost sound attributes
		alSourcef(alSource[6], AL_GAIN, 0.7f);
		alSourcef(alSource[6], AL_PITCH, 1.2f);

		//Powerup sound attributes
		alSourcef(alSource[7], AL_GAIN, 0.7f);
		alSourcef(alSource[7], AL_PITCH, 1.2f);

		//Monster Growl attributes
		alSourcef(alSource[12], AL_GAIN, 0.8f);
		alSourcef(alSource[12], AL_PITCH, 2.0f);

		//Monster Laugh attributes
		alSourcef(alSource[13], AL_GAIN, 0.8f);
		alSourcef(alSource[13], AL_PITCH, 2.0f);
		
		//Monster 3 attributes
		alSourcef(alSource[14], AL_GAIN, 0.8f);
		alSourcef(alSource[14], AL_PITCH, 2.0f);

		//Button1 attributes
		alSourcef(alSource[15], AL_GAIN, 0.8f);
		alSourcef(alSource[15], AL_PITCH, 1.2f);

		//Button2 attributes
		alSourcef(alSource[16], AL_GAIN, 0.8f);
		alSourcef(alSource[16], AL_PITCH, 1.2f);
	
		//endGame attibutes
		alSourcef(alSource[17], AL_GAIN, 0.5f);
		alSourcef(alSource[17], AL_PITCH, 1.0f);
		
		//JohnCena attributes
		alSourcef(alSource[18], AL_GAIN, 0.7f);
		alSourcef(alSource[18], AL_PITCH, 1.0f);
		
		//AlienEnter attributes
		alSourcef(alSource[19], AL_GAIN, 0.7f);
		alSourcef(alSource[19], AL_PITCH, 1.5f);
		
		//AlienExit attributes
		alSourcef(alSource[20], AL_GAIN, 0.7f);
		alSourcef(alSource[20], AL_PITCH, 1.5f);
		
		//ILLUMINATI attributes
		alSourcef(alSource[21], AL_GAIN, 1.0f);
		alSourcef(alSource[21], AL_PITCH, 1.15f);
		alSourcei(alSource[21], AL_LOOPING, AL_TRUE);
		
		//Bat attributes
		alSourcef(alSource[22], AL_GAIN, 0.7f);
		alSourcef(alSource[22], AL_PITCH, 1.5f);
}

void toggle_music(bool play)
{
	if (play == 1) {
	alSourcePlay(alSource[0]);
	//printf("Playing sound %i", alSource[0]);
	}
	else if (play == 0) {
		alSourceStop(alSource[0]);
	}
}

void play_slide(void) 
{
	if (!dead && play){
	alSourcePlay(alSource[3]);
	//printf("Playing sound %d\n", alSource[3]);
	play = !play;
	}
}

void play_dead(void) 
{
	alSourcePlay(alSource[4]);
	//printf("Playing sound %i", alSource[4]);
}

void play_jumpsound(void) 
{
	if (!dead && play){
	alSourcePlay(alSource[5]);
	//printf("Playing sound %i", alSource[6]);
	play = !play;
	}
}

void play_boost(void)
{
	int random = rand() % 100 + 1;
	if (random <= 50) {
		alSourcePlay(alSource[6]);
	} else {
		alSourcePlay(alSource[7]);
	}
}

void play_spears(void)
{
	int rando = rand() % 400 + 1;
	if (rando <= 100) {
		alSourcePlay(alSource[8]);
	}
	else if (rando <= 200) {
		alSourcePlay(alSource[9]);
	}
	else if (rando <= 300) {
		alSourcePlay(alSource[10]);
	} else {
		alSourcePlay(alSource[11]);
	}
}	

void play_monster(void)
{
	int roll = rand() % 75 + 1;
	if (roll <= 25) {
		alSourcePlay(alSource[12]);
	} 
	else if	(roll <= 50) {
		alSourcePlay(alSource[13]);
	} else {
		alSourcePlay(alSource[14]);
	}
}

//Buttons 15, 16
void play_button1(void)
{
	alSourcePlay(alSource[15]);
}

void play_button2(void)
{
	alSourcePlay(alSource[16]);
}

void play_end(void)
{
	alSourcePlay(alSource[17]);
	//printf("Playing sound %i", alSource[17]);
}

//Sound when T button is pushed (18)
void play_king(void)
{
	if ((!dead && king)){
		alSourcePlay(alSource[18]);
		//printf("Playing sound %d\n", alSource[16]);
		king = !king;
	}
}

//Sound when alien ship flies by (19-20)
void play_alien(void) 
{
	if (alien) {
		alSourcePlay(alSource[19]);
		//printf("Playing sound %d\n", alSource[17]);
		alien = !alien;
	}
}

//Sound when creature shows on screen
void play_illuminati(void)
{
	if (confirmed) {
		alSourcePlay(alSource[21]);
		//printf("Playing sound %d\n", alSource[17]);
		confirmed = !confirmed;
	}
}

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
		    fprintf(stderr, "ALUT Error: %s\n", 
			alutGetErrorString(alutGetError()));
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
string oldfile = path + filename + filetype;
string newfile = path + filename + ".ppm";

//converting sprite.png to sprite.ppm
string newstuff =  "convert " + oldfile + " " + newfile;
filename = system(newstuff.data());
return filename;
}

Rect showDave(int location)
{
    Rect text;
    text.bot = 380;
    text.left = location;
    text.center = 0;
    return text;
}

//void Main_Menu(game *g)
//{





//}
