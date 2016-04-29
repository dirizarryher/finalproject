//Ryan Frapwell
//Final Project Code
//This code gets called when the sprite jumps. It updates the y axis of the sprite.

#include <iostream>
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

void Jumping(double spritesheetx, float wid)
{
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f+spritesheetx, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(0.0f+spritesheetx, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(0.111111111f+spritesheetx, 0.0f); glVertex2i( wid,wid);
    glTexCoord2f(0.111111111f+spritesheetx, 1.0f); glVertex2i( wid,-wid);
    //glEnd();
    //glPopMatrix();
}
