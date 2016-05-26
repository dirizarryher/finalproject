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

typedef struct Runner {
	Vec pos;
	Vec vel;
} Runner;
Runner runner;

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

    bool showName;
	bool jumped;
	bool gameOver;
	unsigned long int score;

	Game()
	{
		n = 0;
		obhead = NULL;
		nobstacles = 0;
		gameTimer = 0;
		score = 0;
        showName = false;
		jumped = false;
		gameOver = false;
	}
};

#endif
