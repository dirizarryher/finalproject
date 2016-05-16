// David A. Hernandez II CS335
#include "davidH.h"
#include </usr/include/AL/alut.h>

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
    ALuint alSource;
    ALuint alBuffer;

    alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		get_ALerror(error);
		return;
	}
	//Clear error state
	alGetError();

	//Setup the Listener
    //Forward and up vectors are used
	//float vec[6] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);

	//alBuffer holds the sound information
	alBuffer = alutCreateBufferFromFile("./test.wav");

	//alSource refers to the sound
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

//Creates and load an array of sounds for a soundboard
void load_soundboard(void) {	
    alGenBuffers(NUM_BUFFERS, buffers);
	if ((error = alGetError() != AL_NO_ERROR)) {
		printf("alGenBuffers Error: %d",error);
		return;
	}
	alGetError();

	//Initializing array of sounds
	alutLoadWAVFile("test.wav", &format, &data, &size, &freq, &loop);
    //alutCreateBufferFromFile("./test.wav");
	if ((error = alGetError()) != AL_NO_ERROR) {
		printf("alutLoadWAVFile Error: %d", error);
		//Delete buffers if they don't load
		alDeleteBuffers(NUM_BUFFERS, buffers);
		return;
	}
    alGetError();

	//Fills up buffers with data received from alutLoadWAVFile into a buffer
	//and makes it ready to be assigned to a waiting sources and played
	alBufferData(buffers[0], format, data, size, freq);
	if ((error = alGetError()) != AL_NO_ERROR) {
		printf("alBufferData buffer[0] Error: %d", error);
		//Delete if fails
		alDeleteBuffers(NUM_BUFFERS, buffers);
		return;
	}
    alGetError();

	//Deletes sound data from memory since data is now stored in the buffer
	alutUnloadWAV(format, data, size, freq);
	if(error != AL_NO_ERROR) {
		printf("alutUnloadWAV Error: %d", error);
		//Delete if fails
		alDeleteBuffers(NUM_BUFFERS, buffers);
		return;
	}
    alGetError();

	//Generate the sources
	ALuint source[NUM_SOURCES];
	alGenSources(NUM_SOURCES, source);
	if ((error = alGetError()) != AL_NO_ERROR) {
		printf("alGenSources Error: %d", error);
		return;
	}
    alGetError();

	//Attaches buffer to the source in order to use buffer
	//when the source is played
	alSourcei(source[0], AL_BUFFER, buffers[0]);
	if ((error = alGetError()) != AL_NO_ERROR) {
		printf("alSourcei Error: %d", error);
		return;
	}
    alGetError();

	//Set listener for 3D sounds
	ALvoid alListenerfv(ALenum pname, ALfloat *values);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_DIRECTION, listenerOri);
	//Setting the position of the source to simulate 3D sound
	ALvoid alSourcefv(ALuint source, ALenum pname, ALfloat *values);
	alSourcefv(source[0], AL_POSITION, sourcePos);
	alSourcefv(source[0], AL_VELOCITY, sourceVel);
	alSourcefv(source[0], AL_DIRECTION, sourceOri);

	//Now we play the sound using:
	//alSourcePlay(source[0]);
	//Stopping sound:
	//alSourceStop(souce[0]);
	//Resetting sound to the start of the sample
	//alSourceRewind(souce[0]);
	//Pausing sound:
	//alSourcePause(souce[0]);
}

void clean_music(ALuint &alBuffer, ALuint alSource) 
{
	//First, delete the source
	alDeleteSources(1, &alSource);
	//Delete the buffer
	alDeleteBuffers(1, &alBuffer);

	//Delete the soundboard source and buffer
	alDeleteSources(NUM_SOURCES, source);
	alDeleteBuffers(NUM_BUFFERS, buffers);

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

	//alutExit();
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
