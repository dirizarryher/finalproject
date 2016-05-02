<<<<<<< HEAD
// David A. Hernandez II CS335
#include "Structures.h"
#include <cstdio>
//#include <FMOD/fmod.h>
//#include <FMOD/wincompat.h>
#include <fmod.h>
extern "C" {
#include "fonts.h"
}

Ppmimage *runningImage = NULL;
Ppmimage *jumpImage = NULL;
Ppmimage *deathImage = NULL;
GLuint runnerTexture, jumpTexture, deathTexture;

void getRunnerTexture(void);
void setRunnerTexture(Game *g);

void getRunnerTexture(void)
{
	runningImage = ppm6GetImage("./images/runner/runner_sheet2.ppm");
	jumpImage = ppm6GetImage("./images/runner/runner_sheet2.ppm");
	deathImage = ppm6GetImage("./images/runner/runner_sheet2.ppm");
	runningImage = ppm6GetImage("./images/runner/runner_sheet2.ppm");

	glGenTextures(1, &runnerTexture);
	glGenTextures(1, &jumpTexture);
	glGenTextures(1, &deathTexture);
	
	glBindTexture(GL_TEXTURE_2D, runnerTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER_GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER_GL_NEAREST);
	glTexImage2d(GL_TEXTURE_2D, 0, 3, runningImage->w, runningImage->h,
		0, GL_RGB, GL_UNSIGNED_BYTE, runningImage->data);

	glBindTexture(GL_TEXTURE_2D, jumpTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER_GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER_GL_NEAREST);
	glTexImage2d(GL_TEXTURE_2D, 0, 3, jumpImage->w, jumpImage->h,
		0, GL_RGB, GL_UNSIGNED_BYTE, jumpImage->data);

	glBindTexture(GL_TEXTURE_2D, deathTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER_GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER_GL_NEAREST);
	glTexImage2d(GL_TEXTURE_2D, 0, 3, deathImage->w, deathImage->h,
		0, GL_RGB, GL_UNSIGNED_BYTE, deathImage->data);


}





void init_sounds(void)
{
	if (fmod_init()) {
		printf("ERROR");
		return;
	}
	if (fmod_createsound((char *)"./sounds/untitled.mp3", 0)) {
		printf("ERROR");
		return;
	}
	if (fmod_createsound((char *)"./sounds/untitled2.mp3", 1)) {
		printf("ERROR");
		return;
	}
	fmod_setmode(0, FMOD_LOOP_NORMAL);
	fmod_setmode(1, FMOD_LOOP_NORMAL);
}

void play_music(int a)
{
	fmod_playsound(a);
}

string convertImage(string filename, string path, string filetype)
{
    string oldfile = path + filename + filetype;
    string newfile = path + filename + ".ppm";

    //convert ./images/runner/runner_sheet2.ppm ./images/runner/runner_sheet2.png
    string newstuff =  "convert " + oldfile + " " + newfile;
    filename = system(newstuff.data());
    return filename;

}









bool endGame(Game *g)
{
	//If runner dies, game over
	if (g->runner.dead >= 100)
		return true;
	else
		return false;
}

void endMenu(Game *g)
{
	int yellow = 0x00ffff00;
	int redd = 0x00ff0000;

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
=======
// David A. Hernandez II CS335
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "ppm.h"
using namespace std;

//Defined Types
typedef double Flt;
typedef double Vec[3];
typedef Flt     Matrix[4][4];

typedef struct t_bigfoot {
    Vec pos;
    Vec vel;
} Bigfoot;


string convertImage(string filename, string path, string filetype)
{
    string oldfile = path + filename + filetype;
    string newfile = path + filename + ".ppm";

    //convert ./images/runner/runner_sheet2.ppm ./images/runner/runner_sheet2.png
    string newstuff =  "convert " + oldfile + " " + newfile;
    filename = system(newstuff.data());
    return filename;

}
    



>>>>>>> d51a90ac14d527e44a0c6f66e7cc8dd7937e51aa
