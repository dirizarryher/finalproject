//John Henry Buenaventura
//Initiated April 28
//Updated May 10/////////
//
/*#include <iostream>
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
}*/

#include "tyM.h"

int sliding(double spritesheetx, float wid, int slide, Bigfoot &bigfoot, GLuint slideTexture)
{

    if(slide==1){
        bigfoot.pos[1] -= 3;
        spritesheetx = 0;
        slide++;
    }

    glPushMatrix();
    glTranslatef(bigfoot.pos[0], bigfoot.pos[1], bigfoot.pos[2]);
    glBindTexture(GL_TEXTURE_2D, slideTexture);
    glBegin(GL_QUADS);

    glTexCoord2f(0.01f+spritesheetx, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(0.0f+spritesheetx, 0.01f); glVertex2i(-wid, wid);

    glTexCoord2f(0.111111111f+spritesheetx, 0.0f); glVertex2i( wid,wid);
    glTexCoord2f(0.111111111f+spritesheetx, 1.0f); glVertex2i( wid,+wid);

    glEnd();
    glPopMatrix();

    return slide;

}
