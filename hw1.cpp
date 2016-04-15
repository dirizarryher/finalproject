#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
extern"C" {
#include "fonts.h"
}
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define SEGMENTS 60
#define ADJUST (3.14159 * 2.0) / (float)SEGMENTS
#define rnd() (float)rand() / (float)RAND_MAX
#define MAX_PARTICLES 50000
#define GRAVITY 0.1
int set = 0, direction = -1;

int box_x = 400, box_y = 10, box_length = 80;
//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

//Structures
struct Vec {
    float x, y, z;
};

struct Shape {
    float width, height;
    float radius;
    Vec center;
};

struct Particle {
    Shape s;
    Vec velocity;
};

struct Game {
    Shape box[5];
    Shape circle[2];
    Particle particle[MAX_PARTICLES];
    int n;
    int lastMousex;
    int lastMousey;
};

//Function prototypes
void makeParticle(Game *game, int x, int y);
void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_mouse(XEvent *e, Game *game);
int check_keys(XEvent *e, Game *game);
void movement(Game *game);
void render(Game *game);


int main(void)
{
    int done=0;
    srand(time(NULL));
    initXWindows();
    init_opengl();
    //declare game object
    Game game;
    game.n=0;

    //declare a box shape
    int x = 400, y = 10, length = 800;
    //declare a box shape
    game.box[0].width = length;
    game.box[0].height = 10;
    game.box[0].center.x = x;
    game.box[0].center.y = y;
    game.circle[0].center.x = 10;
    game.circle[0].center.y = -20;
    game.circle[0].radius = 100;


    //start animation
    while(!done) {
	if (direction == -1)
	    direction += 2;
	else
	    direction -= 2;
	while(XPending(dpy)) {
	    XEvent e;
	    XNextEvent(dpy, &e);
	    check_mouse(&e, &game);
	    done = check_keys(&e, &game);
	}
	movement(&game);
	render(&game);
	glXSwapBuffers(dpy, win);
    }
    cleanupXWindows();
    cleanup_fonts();
    return 0;
}

void set_title(void)
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "335 HW1   LMB for particle");
}

void cleanupXWindows(void) {
    //do not change
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

void initXWindows(void) {
    //do not change
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    int w=WINDOW_WIDTH, h=WINDOW_HEIGHT;
    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
	std::cout << "\n\tcannot connect to X server\n" << std::endl;
	exit(EXIT_FAILURE);
    }
    Window root = DefaultRootWindow(dpy);
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if(vi == NULL) {
	std::cout << "\n\tno appropriate visual found\n" << std::endl;
	exit(EXIT_FAILURE);
    } 
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
	ButtonPress | ButtonReleaseMask |
	PointerMotionMask |
	StructureNotifyMask | SubstructureNotifyMask;
    win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
	    InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    set_title();
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);
}

void init_opengl(void)
{
    //for fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
    //OpenGL initialization
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //Set 2D mode (no perspective)
    glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
    //Set the screen background color
    glClearColor(0.1, 0.1, 0.1, 1.0);
    //Do this to allow fonts
}

void makeParticle(Game *game, int x, int y) {
    if (game->n >= MAX_PARTICLES)
	return;
    //position of particle
    Particle *p = &game->particle[game->n];
    p->s.center.x = rand() % 51 + 380;
    p->s.center.y = y;
    p->velocity.y = rnd()*0.1 - 0.5;
    p->velocity.x = direction;                   
    game->n++;
}

void check_mouse(XEvent *e, Game *game)
{
    static int savex = 0;
    static int savey = 0;
    //static int n = 0; took out

    if (e->type == ButtonRelease) {
	return;
    }
    if (e->type == ButtonPress) {
	if (e->xbutton.button==1) {
	    //Left button was pressed
	    int y = WINDOW_HEIGHT - e->xbutton.y;

	    for(int i=0; i<10; i++)
		makeParticle(game, e->xbutton.x, y);
	    return;

	}
	if (e->xbutton.button==3) {
	    //Right button was pressed
	    return;
	}
    }
    //Did the mouse move?
    if (savex != e->xbutton.x || savey != e->xbutton.y) {
	savex = e->xbutton.x;
	savey = e->xbutton.y;
	//int y = WINDOW_HEIGHT - e->xbutton.y;

	game->lastMousex = e->xbutton.x;  
	game->lastMousey = e->xbutton.y;    

    }
}

int check_keys(XEvent *e, Game *game) {
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

    }
    return 0;
}

void movement(Game *game)
{
    Particle *p;
    int val = 0;

    //declare a box shape
    game->box[1].width = box_length;
    game->box[1].height = 20;
    game->box[1].center.x = box_x -= 5;
    game->box[1].center.y = box_y;
    if (val == 0 ) { 
	box_y += 60;
	val = 1;
    }
    else { 
	box_y -= 60;
	val = 0;
    }

    if ( game->box[1].center.x < 0)
	box_x = 800;
    game->lastMousex = 445;
    game->lastMousey = 540;
    if(set){
	for(int i = 0; i < 10; i++)
	    makeParticle(game, game->lastMousex, game->lastMousey);
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

void render(Game *game)
{ 
    float w, h;
    glClear(GL_COLOR_BUFFER_BIT);
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

    //draw all particles here
    for(int i=0; i<game->n; i++){
	glPushMatrix();
	glColor3ub(rnd()*150,rnd()*160,220);
	Vec *c = &game->particle[i].s.center;
	w = 2;
	h = 2;
	glBegin(GL_QUADS);
	glVertex2i(c->x-w, c->y-h);
	glVertex2i(c->x-w, c->y+h);
	glVertex2i(c->x+w, c->y+h);
	glVertex2i(c->x+w, c->y-h);
	glEnd();
	glPopMatrix();
    }
    Rect textbox[5];
    Rect header;
    int top = 600, width = 800;
    glBindTexture(GL_TEXTURE_2D, 0);   

    header.bot =  top - 50;
    header.left = width / 2;
    header.center = -100;      
    for(int i = 0; i < 5; i++) {
	top -= 90;
	textbox[i].bot =  top - 5;
	textbox[i].left = width / 2;
	textbox[i].center = -100;      
    }

    unsigned int color = 0x00ffff;
    ggprint8b(&textbox[0], 16, color, "Requirements");
    ggprint8b(&textbox[1], 16, color, "Design");
    ggprint8b(&textbox[2], 16, color, "Coding");
    ggprint8b(&textbox[3], 16, color, "Testing");
    ggprint8b(&textbox[4], 16, color, "Maintenance");
    ggprint16(&header, 16, 0x00ffffff, "Waterfall Model");   
}
