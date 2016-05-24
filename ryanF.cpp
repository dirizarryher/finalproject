//Ryan Frapwell
// First created 4/25/16
// Last modified 5/13/16

#include "tyM.h"

extern "C" {
#include "fonts.h"
}


//This code gets called when the sprite jumps. 
//It updates the y axis of the sprite to make it look like it is jumping.
int Jumping (double spritesheetx, float wid, int jump, 
	int *sprite_y, GLuint jumpTexture, int stuff)
{
    if (!stuff) {
	if (jump < 32 && jump > 0) {
	    *sprite_y += 3;
	    jump++;
	} else if (jump >= 32 && jump < 62 ) {
	    *sprite_y -= 3;
	    jump ++;
	} else {
	    jump = 0;
	    *sprite_y = 75;
	}
    }
    if (stuff) {
	*sprite_y += 3;
	jump++;
    }

    glPushMatrix();
    //cout << bigfoot.pos[1] << endl;
    glTranslatef(140, *sprite_y, 0);
    glBindTexture(GL_TEXTURE_2D, jumpTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f+spritesheetx, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(0.0f+spritesheetx, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(0.1f+spritesheetx, 0.0f); glVertex2i( wid, wid);
    glTexCoord2f(0.1f+spritesheetx, 1.0f); glVertex2i( wid,-wid);
    glEnd();
    glPopMatrix();

    return jump;

}

//This will project powerups onto the screen in front of the player
void projectImage(float x, float y, float z, GLuint Texture)
{
    float wid = 30;
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTranslatef(x, y, z);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( wid,wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
    glEnd();
    glPopMatrix();

}

int checkcollison(int sprite_x, float x, int sprite_y, float y)
{

    if (x <= sprite_x+50 && x >= sprite_x-50) {
	if (y <= sprite_y+50 && y >= sprite_y-20) {
	    return 1;
	}
    } else {
	return 0; 
    }
    return 0; 
}

void displaybackground(double backgroundx, GLuint Texture, int yres, int xres, bool toggle)
{
    if(!toggle)
    {
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f-backgroundx, 1.0f); glVertex2i(0, 0);
	glTexCoord2f(0.0f-backgroundx, 0.0f); glVertex2i(0, yres);
	glTexCoord2f(1.0f-backgroundx, 0.0f); glVertex2i(xres, yres);
	glTexCoord2f(1.0f-backgroundx, 1.0f); glVertex2i(xres, 0);
	glEnd();
    } else {
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f-backgroundx, 1.0f); glVertex2i(0, 0);
	glTexCoord2f(0.0f-backgroundx, 0.0f); glVertex2i(0, yres);
	glTexCoord2f(1.0f-backgroundx, 0.0f); glVertex2i(xres*2, yres);
	glTexCoord2f(1.0f-backgroundx, 1.0f); glVertex2i(xres*2, 0);
	glEnd();
    }
}

//updats the farbackground x coordinate and makes it scroll
double farbackground(double x)
{
    if (x)
	x-=.0001;
    return x;
}

//updats the background x coordinate and makes it scroll
double backgroundscroll(double x)
{
    if (x)
	x-=.0006;
    return x;
}

//updats the sky x coordinate and makes it scroll
double sky(double x)
{
    if (x)
	x-=.00005;
    return x;
}

//updats the grass x coordinate and makes it scroll
double grass(double x)
{
    if (x)
	x-=.05;
    return x;
}

//updats the ground x coordinate and makes it scroll
double ground(double x)
{
    if (x)
	x-=.005;
    return x;
}

//code taken from Gordon's Rainforest.cpp
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
        //              (int)*(ptr+0) +
        //              (int)*(ptr+1) +
        //              (int)*(ptr+2)) / 3);
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

//code modified from Gordon's Rainforest.cpp
void assignboostTexture(GLuint *Texture, Ppmimage *Image)
{
 int boost_w = Image->width;
    int boost_h = Image->height;
    //
    glBindTexture(GL_TEXTURE_2D, *Texture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, boost_w, boost_h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, Image->data);

    glBindTexture(GL_TEXTURE_2D, *Texture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *BoostsilhouetteData = buildAlphaData(Image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, boost_w, boost_h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, BoostsilhouetteData);

}

//code modified from Gordon's Rainforest.cpp
void assignJumpTexture(GLuint *jumpTexture, Ppmimage *jumpImage)
{
    int jump_w = jumpImage->width;
    int jump_h = jumpImage->height;
    //
    glBindTexture(GL_TEXTURE_2D, *jumpTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, jump_w, jump_h, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, jumpImage->data);

    glBindTexture(GL_TEXTURE_2D, *jumpTexture);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *jumpData = buildAlphaData(jumpImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, jump_w, jump_h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, jumpData);

}

//code modified from Gordon's Rainforest.cpp
void assignbackgroundTexture(GLuint *Texture, Ppmimage *Image)
{
    int tmp_w = Image->width;
    int tmp_h = Image->height;
    //
    glBindTexture(GL_TEXTURE_2D, *Texture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tmp_w, tmp_h, 0,
	    GL_RGB, GL_UNSIGNED_BYTE, Image->data);

    glBindTexture(GL_TEXTURE_2D, *Texture);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *farbackgroundData = buildAlphaData(Image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmp_w, tmp_h, 0,
	    GL_RGBA, GL_UNSIGNED_BYTE, farbackgroundData);

}
