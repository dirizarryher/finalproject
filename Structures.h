#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <time.h>
#include <math.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "ppm.h"
#include "log.h"
extern "C" {
#include "fonts.h"
}

//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
(c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]

//constants
#define WINDOW_WIDTH  2000
#define WINDOW_HEIGHT 1000
#define SEGMENTS 60
#define ADJUST (3.14159 * 2.0) / (float)SEGMENTS
#define MAX_PARTICLES 5000
#define GRAVITY 0.1
#define ALPHA 1

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];
extern int xres;
extern int yres;
extern Display *dpy;
extern Window win;
extern XEvent *e;

struct Runner
{
	Vec pos;
	Vec vel;
	float angle;
	float color[3];
	bool dead;
	Runner() {
		/*VecZero(dir);
		pos[0] = (Flt) (xres / 2);
		pos[1] = (Flt) (yres / 2);
		pos[2] = 0.0f;
		VecZero(vel);
		angle = 0.0;
		color[0] = 1.0;
		color[1] = 1.0;
		color[2] = 1.0;
		superMode = 0;
		damageTaken = 0;*/
	}
};

struct Vecs
{
	float x, y, z;
};

struct Shape
{
	float width, height;
	float radius;
	Vecs center;
};

struct Particle
{
	Shape s;
	Vecs velocity;
};

struct Obstacle
{
    Shape s;
	Vec pos;
	int nverts;
	Flt radius;
	Vec vert[4];
	float angle;
	float rotate;
	float color[3];
	struct Obstacle *prev;
	struct Obstacle *next;
	Obstacle()
	{
		prev = NULL;
		next = NULL;
	}
};

struct Game
{
	Shape box[5];
	Shape circle[2];
	Particle particle[MAX_PARTICLES];
	int n;
	int lastMousex;
	int lastMousey;

	Obstacle *obhead;
	int nobstacles;
	int gameTimer;
	
	bool jump;
	bool gameOver;
	unsigned long int score;

/*	struct timespec rTimer;
	struct timespec obsTimer;
	struct timespec jTimer;
	struct timespec sTimer;
	struct timespec dTimer;
*/
	Game()
	{
		n = 0;
		obhead = NULL;
		nobstacles = 0;
		gameTimer = 0;
		score = 0;
		jump = false;
		gameOver = false;
	}
};

#endif
