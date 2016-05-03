// TyAnthoney Morrell CMPS 356
/* This file will perform the animation of the runner dying and will dispaly an 
   animation of the death once the sprite sheet is completely finished. The 
   sprite sheet will need to be adjusted since something happened to the size
   but this will dispaly an animation for the time being. To end the game hit
   escape  
*/

#include "tyM.h"

void runnerDeath (Bigfoot &bigfoot, double spritesheetx) 
{
    float wid = 60.0f;
    glBegin(GL_QUADS);
    if (bigfoot.vel[0] > 0.0) {
	glTexCoord2f(0.0f+spritesheetx, 1.0f); glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f+spritesheetx, 0.0f); glVertex2i(-wid, wid);
	glTexCoord2f(0.1f+spritesheetx, 0.0f); glVertex2i( wid,wid);
	glTexCoord2f(0.1f+spritesheetx, 1.0f); glVertex2i( wid,-wid);
    } else {
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
	glTexCoord2f(0.1f, 1.0f); glVertex2i( wid, wid);
	glTexCoord2f(0.1f, 0.0f); glVertex2i( wid,-wid);
    }
}
