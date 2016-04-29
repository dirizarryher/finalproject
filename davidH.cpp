// David A. Hernandez II CS335
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
#include "Sprite.h"

//Defined Types
typedef double Flt;
typedef double Vec[3];
typedef Flt     Matrix[4][4];

typedef struct t_bigfoot {
    Vec pos;
    Vec vel;
} Bigfoot;

//Cleaning Up Check Keys Function.
//Will need checkKeys for Menu
void checkKeys(XEvent *e)
{
    static int shift = 0;
    int key = XLookupKeysym(&e->key, 0);
    if (e->type == KeyRelease) {
        if (key == XK_Shift_L || key == XK_Shift_R)
            shift = 0;
            return;
        }
    if (e->type == KeyPress) {
        if (key == XK_Shift_L || key == XK_Shift_R) {
            shift = 1;
            return;
        }
    } else {
        return;
    }

    switch(key) {
        case XK_b:
            showRunner ^= 1;
            if (showRunner) {
                bigfoot.pos[0] = -250.0;
            }
            break;
        case XK_Up:
            if (jump == 0)
                jump = 1;
            break;
        case XK_Space:
            if (jump == 0)
                jump == 1;
            break;
        case XK_Down:
            if (slide == 0)
                slide == 1;
            break;
        case XK_Escape:
            done = 1;
            break;
    }
}
    
/*
Sprite::Sprite(const unsigned int pNumberOfTextures)
{
    textures = new GLuint[pNumberOfTextures];
    textureIndex = 0;
    currentFrame = 0;
    numberOfFrames = 0;
    animationDelay = 0.25f;
    animationElapsed = 0.0f;
    position.x = 0.0f;
    position.y = 0.0f;
    size.height = 0.0f;
    size.width = 0.0f;
    velocity = 0.0f;

    collision = true;
    flipHorizontal = false;
    flipVertical = false;
    visible = false;
    active = false;
    isSpriteSheet = false;


}
Sprite::~Sprite()
{
    delete [] textures;
}

bool Sprite::AddTexture(const char* pImage, const bool pTransparent)
{
    GLuint surface = SOIL_load_OGL_texture(pImage, SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID, 0);
    if (surface == 0) {
        return false;
    } else {
    
    textures[textureIndex] = surface;
    textureIndex++;
    if (textureIndex == 1 && numberOfFrames > 1)
    {
        isSpriteSheet = true;
    } else {
        isSpriteSheet = false;
    }
    transparent = pTransparent;

    return true;
    }
}
*/
