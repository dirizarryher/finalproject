// David A. Hernandez II CS335
#include "davidH.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include </usr/include/AL/alut.h>
#include </usr/include/AL/al.h>

ALfloat listenerPos[] = {0.0f, 0.0f, 0.0f};
ALfloat listenerVel[] = {0.0f, 0.0f, 0.0f};
ALfloat listenerOri[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
ALfloat sourcePos[] = {0.0, 0.0, 0.0};
ALfloat sourceVel[] = {0.0, 0.0, 0.0};
ALfloat sourceOri[] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};

extern Sound sound;

Sound::Sound() 
{
	volumeFlag = false;
	error = 0;
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
	ALuint error = sound.error;

	if ((error = alGetError()) != AL_NO_ERROR) {
		get_ALerror(error);
		return;
	}
	//Clear error state
	alGetError();


	//Setup the initial positions for the listener position,
	//velocity, and orientation
	//
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerf(AL_GAIN, 1.0f);


	//Create the buffers and check for errors
	alGenBuffers(NUM_BUFFERS, sound.buffers);
	if ((sound.error = alGetError()) != AL_NO_ERROR) {
		get_ALerror(sound.error);
		return;
	} else {
		printf("Loading Buffers\n");
	}
	alGetError();

	//buffers[] holds the sound information
	//This is the main menu music
	sound.buffers[0] = alutCreateBufferFromFile("sounds/test.wav");
	//Running Sound
	sound.buffers[1] = alutCreateBufferFromFile("sounds/runner1.wav");
	//Heavy Breathing Sound
	sound.buffers[2] = alutCreateBufferFromFile("sounds/runner2.wav");
	//Sliding Sound
	sound.buffers[3] = alutCreateBufferFromFile("sounds/runner3.wav");
	//Sound for dead runner (screaming)
	sound.buffers[4] = alutCreateBufferFromFile("sounds/runner4.wav");
	//Runner in Pain
	sound.buffers[5] = alutCreateBufferFromFile("sounds/runner5.wav");
	//Runner Jumping
	sound.buffers[6] = alutCreateBufferFromFile("sounds/runner6.wav");

	//Variations of spears thrown
	sound.buffers[7] = alutCreateBufferFromFile("sounds/spear1.wav");
	sound.buffers[8] = alutCreateBufferFromFile("sounds/spear2.wav");
	sound.buffers[9] = alutCreateBufferFromFile("sounds/spear3.wav");
	//Sound of multiple spears thrown
	sound.buffers[10] = alutCreateBufferFromFile("sounds/spearFlurry.wav");
	//Spear thrown from Left to Right
	sound.buffers[11] = alutCreateBufferFromFile("sounds/spearLTR.wav");
	//Spear thrown from Right to Left
	sound.buffers[12] = alutCreateBufferFromFile("sounds/spearRTL.wav");

	//Enter button
	sound.buffers[13] = alutCreateBufferFromFile("sounds/button1.wav");
	//Back button
	sound.buffers[14] = alutCreateBufferFromFile("sounds/button2.wav");

	//Game Over Sound
	sound.buffers[15] = alutCreateBufferFromFile("sounds/endOfGame.wav");
	//Hail to the King Baby!
	sound.buffers[16] = alutCreateBufferFromFile("sounds/HailToTheKing.wav");
	//Homage to DOOM
	sound.buffers[17] = alutCreateBufferFromFile("sounds/OutOfGum.wav");


	//Sources[] refer to the sound
	//Generate a source and store it into their respective buffers
	alGenSources(NUM_SOURCES, sound.sources);
	for (int i = 0; i < 18; i++) {
		alSourcei(sound.sources[i], AL_BUFFER, sound.buffers[i]);

		//Set properties for each sound
		alSourcef(sound.sources[i], AL_GAIN, 1.0f);
		alSourcef(sound.sources[i], AL_PITCH, 1.0f);
		alSourcefv(sound.sources[i], AL_POSITION, sourcePos);
		alSourcefv(sound.sources[i], AL_VELOCITY, sourceVel);
		//Set a looping sound for Main Menu music
		if (i == 0) {
			alSourcei(sound.sources[0], AL_LOOPING, AL_TRUE);
		}
		else
			alSourcei(sound.sources[i], AL_LOOPING, AL_FALSE);
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

	for (int i = 0; i < 4; i++) {
		alSourcePlay(sound.sources[0]);
		usleep(250000);
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

void play_sound(ALuint alSource) 
{
	alSourcePlay(alSource);
}

void clean_music(void) 
{
	//Sound sound;
	//Delete the soundboard source and buffer
	alDeleteSources(NUM_SOURCES, sound.sources);
	alDeleteBuffers(NUM_BUFFERS, sound.buffers);

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
