//John Henry Buenaventura
//Initiated April 28
//Updated May 10/////////
//Updated May 13/////////
//Updated May 14/////////
//Updated May 16/////////
//Updated May 19/////////
//
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

//Ppmimage *slideImage2;

//GLuint slideTexture;

typedef double Vec[3];

typedef struct t_bigfoot
{
    Vec pos;
    Vec vel;
}Bigfoot;

/*void slidegl(int slide_w, int slide_h)
{

    slideImage2 = ppm6GetImage("./images/slide_sheet.ppm");

    glGenTextures(1, &slideTexture);

    glBindTexture(GL_TEXTURE_2D, slideTexture);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, slide_w, slide_h, 0, 
            GL_RGB, GL_UNSIGNED_BYTE, slideImage2->data);
}
*/

int sliding(int slidecount, double slidesheetx, float wid, int slide, Bigfoot &bigfoot, GLuint slideTexture)
{

    if(slide < 32 && slide > 0)
    {
        //bigfoot.pos[1];
        slide++;
    }
    else
    {
        slide = 0;
        slidesheetx = 0;
    }

    slidecount++;
    slidesheetx++;

    if(slidecount == 3)
    {
        slidesheetx += 1;
        slidecount = 0;
    }


    glPushMatrix();
    glTranslatef(bigfoot.pos[0], bigfoot.pos[1], bigfoot.pos[2]);
    glBindTexture(GL_TEXTURE_2D, slideTexture);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f+slidesheetx, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(0.0f+slidesheetx, 0.01f); glVertex2i(-wid, wid);

    glTexCoord2f(0.111111111f+slidesheetx, 0.0f); glVertex2i( wid,wid);
    glTexCoord2f(0.111111111f+slidesheetx, 1.0f); glVertex2i( wid,-wid);

    glEnd();
    glPopMatrix();

    return slide;

}
