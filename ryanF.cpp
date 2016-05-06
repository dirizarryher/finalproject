//Ryan Frapwell
// First created 4/25/16
// Last modified 5/4/16

#include "tyM.h"

//This code gets called when the sprite jumps. It updates the y axis of the sprite to make it look like it is jumping.
int Jumping (double spritesheetx, float wid, int jump, Bigfoot &bigfoot, GLuint jumpTexture)
{
    if(jump < 32 && jump > 0) {
	bigfoot.pos[1] +=3;
	jump++;
    } else if(jump >= 32 && jump < 62 ) {
	bigfoot.pos[1] -=3;
	jump ++;
    } else{
	jump = 0;
	bigfoot.pos[1] = 75;
	spritesheetx=0;
    }
    glPushMatrix();
    glTranslatef(bigfoot.pos[0], bigfoot.pos[1], bigfoot.pos[2]);
    glBindTexture(GL_TEXTURE_2D, jumpTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f+spritesheetx, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(0.0f+spritesheetx, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(0.1f+spritesheetx, 0.0f); glVertex2i( wid,wid);
    glTexCoord2f(0.1f+spritesheetx, 1.0f); glVertex2i( wid,-wid);
    glEnd();
    glPopMatrix();

    return jump;

}

//This will project powerups onto the screen in front of the player
void projectImage(float x, float y, float z, GLuint speedTexture)
{
    float wid = 60;
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, speedTexture);
    glTranslatef(x, y, z);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( wid,wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
    glEnd();
    glPopMatrix();

}
