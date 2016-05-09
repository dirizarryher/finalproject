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
#include "Structures.h"
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
double backgroundx = 0, spritesheetx = 0, deathsheetx = 0;
double jumpsheetx = 0;

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

int tmp = 0;
int done=0;
int xres=800, yres=600;


typedef struct t_background {
	Vec pos;
	Vec vel;
} Background;
Background background;
GLuint forestTexture;
Runner runner;

char cScore[400];
int name = 0;
int score = 0;
int distance = 0;
int showRunner=1;
int runnerSpeed = 80000;
int dead=0;
int forest=1;
int silhouette=1;
int trees=1;
int showRain=0;
int deathCounter = 0;
char user[20];

Ppmimage *idleImage, *runningImage, *jumpImage, *deathImage, *boostImage;
GLuint idleTexture, runnerTexture, jumpTexture, deathTexture, boostTexture;
GLuint idleSilhouetteTexture, runnerSilhouetteTexture, jumpSilhouetteTexture,
       deathSilhouetteTexture, boostSilhouetteTexture;

//function prototypes
void Jumping(double spritesheetx, float wid);
//void runnerDeath (Runner &r, double s);
void initXWindows(void);
void initOpengl(void);
void cleanupXWindows(void);
void checkResize(XEvent *e);
void checkMouse(XEvent *e, Game *g);
void checkMouse(XEvent *e);
void checkKeys(XEvent *e);
void init(Game *g);
void physics(void);
void render(Game *game);
bool check_Gamekeys(XEvent *e, Game *game);
void movement(Game *game);
void getRunnerTexture();
int main(void)
{
	initXWindows();
	initOpengl();
	//init_sounds();
	//getRunnerTexture();
	logOpen();
	//init();
	//srand(time(NULL));
	Game game;
	game.n=0;

	//Menu Loop?
	while (done != 1) {
		while (XPending(dpy)) {
			XEvent e;
			XNextEvent(dpy, &e);
			checkResize(&e);
			//checkMouse(&e, &game);
			checkKeys(&e);
			//keys = check_Gamekeys(&e, &game);
/*      	if (argc > 1) {
				string skip = argv[1];
				transform(skip.begin(), skip.end(), skip.begin(), ::tolower);
				if (skip == "skip") {
					done = 3;
				}
				else {
					cout << "You can also skip the start menu with " << argv[0] << " skip " << endl;
					done = check_keys(&e);
				}
			}
			else {
				done = check_keys(&e);
				cout << "You can also skip the start menu with: " << argv[0] << " skip " << endl;
			}
            */
		}
		Rect r;
		r.bot = yres - 100;
		r.left = 500;
		r.center = 0;
		ggprint16(&r, 36, 0xffffffff, "Campus Run");
		r.bot = yres - 500;
		r.left = 250;
		ggprint16(&r, 36, 0xffffffff, "[S]tart Game	[C]ontrols	[O]ptions	[ESC] Exit Game");
		//If user selects Controls
		if (done == 2) {
			r.bot = yres - 200;
			r.left = 250;
			ggprint16(&r, 36, 0xffffffff, "Press [Up] to jump!");
			ggprint16(&r, 36, 0xffffffff, "Press [Down] to slide!");
		}
		//If user selects Options
		if (done == 5) {
			r.bot = yres - 200;
			r.left = 675;
			ggprint16(&r, 36, 0xffffffff, "Race against other users for the highest score!");
					ggprint16(&r, 36, 0xffffffff, "Jump or slide under obstacles to continue running!");
						ggprint16(&r, 36, 0xffffffff, "Watch out for that tree, George!");
			ggprint16(&r, 36, 0xffffffff, "Idk what other instructions to give you. It's pretty straightforward.");
		}
		//If user selects Start Game
		if (done == 3) {
			//play_music(0);
			//init(&game);
			break;
		}
		glXSwapBuffers(dpy, win);
	}
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	
    //Game Loop
	while (done != 1) {
		while (XPending(dpy)) {
			XEvent e;
			XNextEvent(dpy, &e);
			checkResize(&e);
			//checkMouse(&e, &game);
            checkKeys(&e);
			//done = checkKeys(&e);
			//keys = check_Gamekeys(&e, &game);
		}
		//Below is a process to apply physics at a consistent rate.
		//1. Get the time right now.
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		//2. How long since we were here last?
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		//3. Save the current time as our new starting time.
		timeCopy(&timeStart, &timeCurrent);
		//4. Add time-span to our countdown amount.
		physicsCountdown = runnerSpeed;
		//5. Has countdown gone beyond our physics rate? 
		//       if yes,
		//           In a loop...
		//              Apply physics
		//              Reducing countdown by physics-rate.
		//              Break when countdown < physics-rate.
		//       if no,
		//           Apply no physics this frame.
		while (physicsCountdown >= physicsRate) {
			//6. Apply physics
			physics();
			//7. Reduce the countdown by our physics-rate
			physicsCountdown -= physicsRate;
		}
		//Always render every frame.
		counter++;
		movement(&game);
		render(&game);
		glXSwapBuffers(dpy, win);
	}
	cleanupXWindows();
	cleanup_fonts();
	logClose();
	return 0;
}

void cleanupXWindows(void)
{
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}
void setTitle(void)
{
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "CS335 - OpenGL Animation Template Under XWindows");
}
void setupScreenRes(const int w, const int h)
{
	xres = w;
	yres = h;
}
void initXWindows(void)
{
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
	XSetWindowAttributes swa;

	setupScreenRes(640, 480);
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		printf("\n\tcannot connect to X server\n\n");
		exit(EXIT_FAILURE);
	}
	Window root = DefaultRootWindow(dpy);
	XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	if (vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		exit(EXIT_FAILURE);
	} 
	Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
		StructureNotifyMask | SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, xres, yres, 0,
			vi->depth, InputOutput, vi->visual,
			CWColormap | CWEventMask, &swa);
	GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
	setTitle();
}

void reshapeWindow(int width, int height)
{
	//window has been resized.
	setupScreenRes(width, height);
	//
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, xres, 0, yres, -1, 1);
	setTitle();
}

unsigned char *buildAlphaData(Ppmimage *img)
{
	//add 4th component to RGB stream...
	int i;
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		//get largest color component...
		//*(ptr+3) = (unsigned char)((
		//		(int)*(ptr+0) +
		//		(int)*(ptr+1) +
		//		(int)*(ptr+2)) / 3);
		//d = a;
		//if (b >= a && b >= c) d = b;
		//if (c >= a && c >= b) d = c;
		//*(ptr+3) = d;
		*(ptr+3) = (a|b|c);
		ptr += 4;
		data += 3;
	}
	return newdata;
}

void initOpengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, xres, yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, xres, 0, yres, -1, 1);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);

	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	initialize_fonts();
    getRunnerTexture();
	//load_background();

/*
	//forest
	//int forest_w = WINDOW_WIDTH * 2;
	//int forest_h = WINDOW_HEIGHT;
	glBindTexture(GL_TEXTURE_2D, forestTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
			forestImage->width, forestImage->height,
			0, GL_RGB, GL_UNSIGNED_BYTE, forestImage->data);
	//-------------------------------------------------------------------------
    */
}
void checkResize(XEvent *e)
{
	//The ConfigureNotify is sent by the
	//server if the window is resized.
	if (e->type != ConfigureNotify)
		return;
	XConfigureEvent xce = e->xconfigure;
	if (xce.width != xres || xce.height != yres) {
		//Window size did change.
		reshapeWindow(xce.width, xce.height);
	}
}
void initSounds(void)
{
	//You may add sound here for some extra credit.
	//Fmod is not allowed.
	//OpenAL sound only.
	//Look for the openalTest folder under code.
}



void init() {
	//Initialize boxes here
	for (int i = 0; i < 5; i++) {
		Obstacle *ob = new Obstacle;
		ob->nverts = 4;	
		ob->s.width = 50;
		ob->s.height = 50;

	}




	MakeVector(-150.0,180.0,0.0, runner.pos);
	MakeVector(6.0,0.0,0.0, runner.vel);
}

void checkMouse(XEvent *e)
{
	static int savex = 0;
	static int savey = 0;

	if (e->type == ButtonRelease) {
		return;
	}

	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
}

/*bool check_Gamekeys(XEvent *e, Game *game) {
  if (e->type == KeyPress) {
  int key = XLookupKeysym(&e->xkey, 0);
  if (key == XK_Escape) {
  return 1;
  }
//You may check other keys here
if (key == XK_s) {
set ^= 1 ;
return 0; 
}

if (key == XK_Up) {
if(jump == 0)
jump = 1;
return 0; 
}

}
return 0;
}*/

void checkKeys(XEvent *e)
{
	//keyboard input?
	static int shift=0;
	int key = XLookupKeysym(&e->xkey, 0);
	if (e->type == KeyRelease) {
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift=0;
		return;
	}
	if (e->type == KeyPress) {
		if (key == XK_Shift_L || key == XK_Shift_R) {
			shift=1;
			return;
		}
	} else {
		return;
	}
	switch(key) {
		case XK_b:
			showRunner^= 1;
			if (showRunner) {
				runner.pos[0] = -250.0;
			}
			break;
		case XK_d:
			dead ^= 1;
			showRunner = 0;
			runnerSpeed = 1000000;
			break;
		case XK_f:
			forest ^= 1;
			break;
		case XK_s:
			silhouette ^= 1;
			printf("silhouette: %i\n",silhouette);
			break;
		case XK_t:
			trees ^= 1;
			break;
		case XK_u:
			break;
		case XK_p:
			break;
		case XK_r:
			break;
		case XK_Left:
			break;
		case XK_Right:
			break;
		case XK_Down:
			break;
		case XK_equal:
			break;
		case XK_minus:
			break;
		case XK_n:
			break;
		case XK_w:
			break;
		case XK_Up:
			if(jump == 0)
				jump = 1;
			break;
		case XK_Escape:
			done=1;
			break;
	}
}

Flt VecNormalize(Vec vec)
{
	Flt len, tlen;
	Flt xlen = vec[0];
	Flt ylen = vec[1];
	Flt zlen = vec[2];
	len = xlen*xlen + ylen*ylen + zlen*zlen;
	if (len == 0.0) {
		MakeVector(0.0,0.0,1.0,vec);
		return 1.0;
	}
	len = sqrt(len);
	tlen = 1.0 / len;
	vec[0] = xlen * tlen;
	vec[1] = ylen * tlen;
	vec[2] = zlen * tlen;
	return(len);
}

void moveBigfoot()
{
	//move bigfoot...
	int addgrav = 1;
	//Update position
	runner.pos[0] = sprite_x;
	runner.pos[1] = sprite_y;
	//Check for collision with window edges
	if ((runner.pos[0] < -140.0 && runner.vel[0] < 0.0) ||
			(runner.pos[0] >= (float)xres+140.0 && runner.vel[0] > 0.0)) {
		runner.vel[0] = -runner.vel[0];
		addgrav = 0;
	}
	if ((runner.pos[1] < 150.0 && runner.vel[1] < 0.0) ||
			(runner.pos[1] >= (float)yres && runner.vel[1] > 0.0)) {
		runner.vel[1] = -runner.vel[1];
		addgrav = 0;
	}
	//Gravity
	if (addgrav)
		runner.vel[1] -= 0.75;
}

void physics(void)
{
	if (showRunner)
		moveBigfoot();
}

//Deletes obstacle when it moves off the screen
void deleteObstacle(Game *g, Obstacle *node)
{
	if (node->prev == NULL) {
		if (node->next == NULL) {
			g->obhead = NULL;
		} else {
			node->next->prev = NULL;
			g->obhead = node->next;
		}
	} else {
		if (node->next == NULL) {
			node->prev->next = NULL;
		} else {
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
	}
	delete node;
	node = NULL;
}

void render(Game *game)
{
	Rect r;

	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//
	//

    //draw a quad with texture
	float wid = 60.0f;
	glColor3f(1.0, 1.0, 1.0);
	if (forest) {
		glBindTexture(GL_TEXTURE_2D, forestTexture);
		glBegin(GL_QUADS);
		/*glTexCoord2f(0.0f, 1.0f); glVertex2i(backgroundx, 0);
		  glTexCoord2f(0.0f, 0.0f); glVertex2i(backgroundx, yres);
		  glTexCoord2f(1.0f, 0.0f); glVertex2i((xres*2)+backgroundx, yres);
		  glTexCoord2f(1.0f, 1.0f); glVertex2i((xres*2)+backgroundx, 0);*/
		glTexCoord2f(0.0f-backgroundx, 1.0f); glVertex2i(0, 0);
		glTexCoord2f(0.0f-backgroundx, 0.0f); glVertex2i(0, yres);
		glTexCoord2f(1.0f-backgroundx, 0.0f); glVertex2i(xres, yres);
		glTexCoord2f(1.0f-backgroundx, 1.0f); glVertex2i(xres, 0);
		glEnd();
	}
	if(jump){
		glPushMatrix();
		glTranslatef(runner.pos[0], runner.pos[1], runner.pos[2]);
		if (!silhouette) {
			glBindTexture(GL_TEXTURE_2D, jumpTexture);
		} else {
			glBindTexture(GL_TEXTURE_2D, jumpSilhouetteTexture);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(255,255,255,255);
		}

		if(jump == 1)
			spritesheetx=0;
		if(jump < 32 && jump > 0) {
			sprite_y += 3;
			jump++;
			//cout << "jump = " << jump <<endl;
		} else if(jump >= 32 && jump < 62 ) {
			sprite_y -= 3;
			jump ++;
			//cout << "jump = " << jump <<endl;
		} else{
			jump = 0;
			sprite_y = 75;
			spritesheetx=0;
			//cout << "jump = " << jump <<endl;
			//cout << "in jump end" <<endl;
		}
		Jumping(spritesheetx, wid);
	}

	if(!jump) {
		if (runner.vel[0] > 0.0) {
			glTexCoord2f(0.0f+spritesheetx, 1.0f); glVertex2i(-wid,-wid);
			glTexCoord2f(0.0f+spritesheetx, 0.0f); glVertex2i(-wid, wid);
			glTexCoord2f(0.111111111f+spritesheetx,0.0f);glVertex2i( wid,wid);
			glTexCoord2f(0.111111111f+spritesheetx,1.0f);glVertex2i( wid,-wid);
		} else {
			glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
			glTexCoord2f(0.1f, 1.0f); glVertex2i( wid, wid);
			glTexCoord2f(0.1f, 0.0f); glVertex2i( wid,-wid);
		}
		glEnd();
		glPopMatrix();
	}
	backgroundx-=.005;
	if(dead) {
		glPushMatrix();
		glTranslatef(runner.pos[0], runner.pos[1], runner.pos[2]);
		if (!silhouette) {
			glBindTexture(GL_TEXTURE_2D, deathSilhouetteTexture);
		} else {
			glBindTexture(GL_TEXTURE_2D, deathSilhouetteTexture);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(255,255,255,255);
		}
		//runnerDeath(runner, deathsheetx);
		glEnd();
		glPopMatrix();
		deathCounter++;
		if (deathCounter == 10)
			dead = 0;
	}
	deathsheetx += .1111;
	if (showRunner) {
		glPushMatrix();
		glTranslatef(runner.pos[0], runner.pos[1], runner.pos[2]);
		if (!silhouette) {
			glBindTexture(GL_TEXTURE_2D, runnerTexture);
		} else {
			glBindTexture(GL_TEXTURE_2D, runnerSilhouetteTexture);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(255,255,255,255);
		}
		glBegin(GL_QUADS);
		sleep(.9);
		if (runner.vel[0] > 0.0) {
			glTexCoord2f(0.0f+spritesheetx, 1.0f); glVertex2i(-wid,-wid);
			glTexCoord2f(0.0f+spritesheetx, 0.0f); glVertex2i(-wid, wid);
			glTexCoord2f(0.111111111f+spritesheetx, 0.0f);glVertex2i(wid,wid);
			glTexCoord2f(0.111111111f+spritesheetx, 1.0f);glVertex2i(wid,-wid);
		} else {
			glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
			glTexCoord2f(0.1f, 1.0f); glVertex2i( wid, wid);
			glTexCoord2f(0.1f, 0.0f); glVertex2i( wid,-wid);
		}
		glEnd();
		glPopMatrix();
		//
	}
	spritesheetx += .11111111111;



	float w, h;
	//Draw shapes...
	//
	//circle
	Shape *s;
	float rad = 0.0;
	int x = 0, y = 0;
	glColor3ub(90,140,90);
	for (int i = 0; i < SEGMENTS; i++) {
		for (int j = 0; j < 2; j++){
			s = &game->circle[j];
			x = cos(ADJUST) * s->radius + s->center.x;
			y = sin(ADJUST) * s->radius + s->center.y;
			rad += ADJUST;
			glVertex2f(x, y);
		}
	}

	//draw box
	glColor3ub(90,140,90);
	for (int j=0; j<5; j++){
		s = &game->box[j];
		glPushMatrix();
		glTranslatef(s->center.x, s->center.y, s->center.z);
		w = s->width;
		h = s->height;
		glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
		glEnd();
		glPopMatrix();
	}


	glDisable(GL_TEXTURE_2D);
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glBegin(GL_QUADS);
	//	glVertex2i(10,10);
	//	glVertex2i(10,60);
	//	glVertex2i(60,60);
	//	glVertex2i(60,10);
	//glEnd();
	//return;
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
    r.bot = yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0, "B - Bigfoot");
	ggprint8b(&r, 16, 0, "F - Forest");
	ggprint8b(&r, 16, 0, "S - Silhouette");
	ggprint8b(&r, 16, 0, "T - Trees");
	ggprint8b(&r, 16, 0, "U - Umbrella");
	ggprint8b(&r, 16, 0, "R - Rain");
	ggprint8b(&r, 16, 0, "D - Deflection");
	ggprint8b(&r, 16, 0, "N - Sounds");
}
void movement(Game *game)
{
	Particle *p;

	//declare a box shape
	/*game->box[1].width = box_length;
	  game->box[1].height = 40;
	  game->box[1].center.x = box_x -= 3;
	  game->box[1].center.y = box_y;*/

	if (jump > 0) {
		game->box[2].center.y += 2;
		jump++;
		if (jump == 51)
			jump = -50;
	} 
	else {
		if(game->box[2].center.y > sprite_y) {
			game->box[2].center.y -= 2;
			sleep(.8);
			jump ++;
		}
	}
	/*if (val == 0 ) { 
	  box_y += 60;
	  }
	  else { 
	  box_y -= 60;
	  }*/

	if ( game->box[1].center.x < -40)
		box_x = 1500;
	game->lastMousex = 445;
	game->lastMousey = 540;
	if(set){
		//for(int i = 0; i < 10; i++)
		//makeParticle(game, game->lastMousex, game->lastMousey);
	}
	if (game->n <= 0)
		return;
	int box_num = 5;
	for(int i=0; i<game->n; i++){
		p = &game->particle[i];
		p->s.center.x += p->velocity.x;
		p->s.center.y += p->velocity.y;
		p->velocity.y -= GRAVITY;

		for (int b = 0; b < box_num; b++) {
			Shape *s = &game->box[b];	 
			if (p->s.center.y >= s->center.y - (s->height) &&
					p->s.center.y <= s->center.y + (s->height) &&
					p->s.center.x >= s->center.x - (s->width) &&
					p->s.center.x <= s->center.x + (s->width)) {
				p->velocity.y *= -0.45;
				p->s.center.y = s->center.y + s->height + 0.01;
			}
		}
		//check circle collison
		float d0, d1, dist;
		for (int j = 0; j < 2; j++) {
			d0 = p->s.center.x - game->circle[j].center.x;
			d1 = p->s.center.y - game->circle[j].center.y;
			dist = sqrt(d0*d0 + d1*d1);
			if(dist <= game->circle[j].radius) {
				d0 /= dist;
				d1 /= dist;
				d0 *= game->circle[j].radius * 1.01;
				d1 *= game->circle[j].radius * 1.01;
				p->s.center.x = game->circle[j].center.x + d0;
				p->s.center.y = game->circle[j].center.y + d1;
				p->velocity.x += d0 * 0.02;
				p->velocity.y += d0 * 0.02;
			}
		}
		if (p->s.center.y < 0.0) {
			game->particle[i] = game->particle[game->n-1];
			game->n--;
		}
	}
}
