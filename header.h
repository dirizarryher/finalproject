//cs335
//
//program: rainforest
//author:  Gordon Griesel
//date:    2013 to present
//
//This program demonstrates the use of OpenGL and XWindows
//
//Texture maps are displayed.
//Press B to see bigfoot roaming his forest.
//
//The rain builds up like snow on the ground.
//Fix it by removing each raindrop for the rain linked list.
//look for the 
//
//
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
extern "C" {
#include "fonts.h"
}

#define WINDOW_WIDTH  2000
#define WINDOW_HEIGHT 1000
#define SEGMENTS 60
#define ADJUST (3.14159 * 2.0) / (float)SEGMENTS
#define MAX_PARTICLES 5000
#define GRAVITY 0.1

int set = 0, direction = -1, jump = 0, counter = 0;
int box_x = 400, box_y = 60, box_length = 40, val = 0,
    sprite_x = 140, sprite_y = 75;


//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];
double backgroundx = 0, spritesheetx = 0, deathsheetx = 0;

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
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define ALPHA 1

//X Windows variables
Display *dpy;
Window win;


//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 30.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown=0.0;
double timeSpan=0.0;
unsigned int upause=0;
double timeDiff(struct timespec *start, struct timespec *end) {
    return (double)(end->tv_sec - start->tv_sec ) +
	(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source) {
    memcpy(dest, source, sizeof(struct timespec));
}
//-----------------------------------------------------------------------------


int done=0;
int xres=800, yres=600;

typedef struct t_background {
    Vec pos;
    Vec vel;
} Background;
Background background;

typedef struct t_bigfoot {
    Vec pos;
    Vec vel;
} Bigfoot;
Bigfoot bigfoot;

Ppmimage *runningImage, *deathImage, *jumpImage;
Ppmimage *forestImage=NULL;
GLuint bigfootTexture, bigfootTexture2, bigfootTexture3;
GLuint silhouetteTexture, DeathsilhouetteTexture, jumpTexture;
GLuint forestTexture;
int showRunner=1;
int runnerSpeed = 80000;
int dead=0;
int forest=1;
int silhouette=1;
int trees=1;
int showRain=0;
int deathCounter = 0;
//
typedef struct t_raindrop {
    int type;
    int linewidth;
    int sound;
    Vec pos;
    Vec lastpos;
    Vec vel;
    Vec maxvel;
    Vec force;
    float length;
    float color[4];
    struct t_raindrop *prev;
    struct t_raindrop *next;
} Raindrop;
Raindrop *ihead=NULL;
int ndrops=1;
int totrain=0;
int maxrain=0;
void deleteRain(Raindrop *node);
void cleanupRaindrops(void);
//
#define UMBRELLA_FLAT  0
#define UMBRELLA_ROUND 1
typedef struct t_umbrella {
    int shape;
    Vec pos;
    Vec lastpos;
    float width;
    float width2;
    float radius;
} Umbrella;
Umbrella umbrella;
int showUmbrella=0;
int deflection=0;

//Structures
struct Vecs {
    float x, y, z;
};

struct Shape {
    float width, height;
    float radius;
    Vecs center;
};

struct Particle {
    Shape s;
    Vecs velocity;
};

struct Game {
    Shape box[5];
    Shape circle[2];
    Particle particle[MAX_PARTICLES];
    int n;
    int lastMousex;
    int lastMousey;
};

//function prototypes
void Jumping(double spritesheetx, float wid);
void runnerDeath (Bigfoot &b, double s);
void initXWindows(void);
void initOpengl(void);
void cleanupXWindows(void);
void checkResize(XEvent *e);
void checkMouse(XEvent *e);
void checkKeys(XEvent *e);
void init();
void physics(void);
void render(Game *game);
int check_Gamekeys(XEvent *e, Game *game);
void movement(Game *game);
