// TyAnthoney Morrell CMPS 356
/* This file will perform the animation of the runner dying and will dispaly an 
   animation the death once the sprite sheet is completely finished. It will 
   work now however. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
////#include <GL/gl.h>
////#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "ppm.h"

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt     Matrix[4][4];
//double backgroundx = 0, spritesheetx = 0;

typedef struct t_bigfoot {
        Vec pos;
	Vec vel;
} Bigfoot;

void runnerDeath (Bigfoot &bigfoot, double spritesheetx) {
    float wid = 60.0f;
    sleep(.99);
    glBegin(GL_QUADS);
    if (bigfoot.vel[0] > 0.0) {
	glTexCoord2f(0.0f+spritesheetx, 1.0f); glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f+spritesheetx, 0.0f); glVertex2i(-wid, wid);
	glTexCoord2f(0.111111111f+spritesheetx, 0.0f); glVertex2i( wid,wid);
	glTexCoord2f(0.111111111f+spritesheetx, 1.0f); glVertex2i( wid,-wid);
    } else {
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
	glTexCoord2f(0.1f, 1.0f); glVertex2i( wid, wid);
	glTexCoord2f(0.1f, 0.0f); glVertex2i( wid,-wid);
    }
}
