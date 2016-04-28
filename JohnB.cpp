//John Henry Buenaventura
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "ppm.h"
extern "C" {
#include "fonts.h"
}


void sliding(double spritesheetx, float wid)
{
    glTexCoord2f(0.01f+spritesheetx, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(0.0f+spritesheetx, 0.01f); glVertex2i(-wid, wid);
    
    glTexCoord2f(0.111111111f+spritesheetx, 0.0f); glVertex2i( wid,wid);
    glTexCoord2f(0.111111111f+spritesheetx, 1.0f); glVertex2i( wid,+wid);
    
    glEnd();
    glPopMatrix();

}
