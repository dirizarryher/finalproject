//John Henry Buenaventura
//Initiated April 28
//Updated May 14/////////
//Updated May 16/////////
//Updated May 19/////////
//Updated May 25/////////
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

    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(0.0f, 0.01f); glVertex2i(-wid, wid);

    glTexCoord2f(1.0, 0.0f); glVertex2i( wid,wid);
    glTexCoord2f(1.0, 1.0f); glVertex2i( wid,-wid);

    glEnd();
    glPopMatrix();

    return slide;

}

//Derived from Lab
unsigned char *buildAlphaData2(Ppmimage *img)
{
    int i;
    int a, b, c;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    newdata = (unsigned char *)malloc(img->width * img->height * 4);
    ptr = newdata;
    for (i = 0; i<img->width * img->height * 3; i+=3)
    {
        a = *(data+0);
        b = *(data+1);
        c = *(data+1);
        //
        *(ptr+0) = a;
        *(ptr+1) = b;
        *(ptr+2) = c;
        *(ptr+3) = (a|b|c);
        //
        ptr += 4;
        data += 3;
    }
    return newdata;
}

//Derived from Lab
void initiateSlideTexture(GLuint *slideTexture, Ppmimage *slideImage)
{
    int slide_w = slideImage->width;
    int slide_h = slideImage->height;

    glBindTexture(GL_TEXTURE_2D, *slideTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, slide_w, slide_h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, slideImage->data);

    glBindTexture(GL_TEXTURE_2D, *slideTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char *slideData = buildAlphaData2(slideImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, slide_w, slide_h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, slideData);
}
    
