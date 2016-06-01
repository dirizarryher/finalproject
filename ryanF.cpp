//Ryan Frapwell
// First created 4/25/16
// Last modified 5/13/16

#include "tyM.h"

extern "C" {
#include "fonts.h"
}

string convertImage(string filename, string path, string filetype);

//This code gets called when the sprite jumps. 
//It updates the y axis of the sprite to make it look like it is jumping.
void display(double diff, int jump, int *score, int* dead, int flag)
{
	Rect Box;
	char name[100];
	Box.bot = 250*diff;
	Box.left = 350*diff;
	Box.center = 0;

	if (!flag) {
		if (jump > 0) {
			sprintf(name, "Ryan Frapwell says don't press that button.");
		}
		if (jump > 100) {
			sprintf(name, "Please press the 'f' button again to go back");
		}
		if (jump > 200) {
			sprintf(name, "Ok, now this is just ridiculous");
		}
		if (jump > 300) {
			sprintf(name, "I know you think you're being clever, but you aren't");
		}
		if (jump > 400) {
			sprintf(name, "Don't make me start subtracting your score");
		}
		if (jump > 500) {
			sprintf(name, "That is it! Here I go");
			*score -=200;
		}
		if (jump > 800) {
			sprintf(name, "Why are you doing this to me?");
		}
		if (jump > 900) {
			sprintf(name, "I spent a lot of time on this game");
		}
		if (jump > 1000) {
			sprintf(name, "Just play the game normally will you?");
		}
		if (jump > 1200) {
			sprintf(name, "There is no prize for getting the lowest score");
		}
		if (jump > 1400) {
			sprintf(name, "whatever.... have it your way");
		}
		if (jump > 1500) {
			sprintf(name, " ");
		}
	}
	if (flag) {
		if (jump > 0) {
			sprintf(name, "Trying to use the cheat again I see?");
		}
		if (jump > 200) {
			sprintf(name, "You hurt my feelings dear cheater");
		}
		if (jump > 300) {
			sprintf(name, "You thought this would work?");
		}
		if (jump > 500) {
			sprintf(name, "I am the one who knocks!");
		}
		if (jump > 600) {
			sprintf(name, "And I also control you're score too. Just saying");
			*score -=200;
		}
		if (jump > 700) {
			sprintf(name, "Ready to admit defeat?");
		}
		if (jump > 900) {
			sprintf(name, "I'll take that as a yes");
		}
		if (jump > 1000) {
			sprintf(name, " ");
			*dead = 1;
		}
	}
	ggprint8b(&Box, 300, 10, name);
}

int Jumping (double spritesheetx, float wid, int jump, int *sprite_y, GLuint jumpTexture, int stuff, double diff, int *score, int* dead, int* flag)
{
	if (!stuff) {
		if (jump < 26 && jump > 0) {
			*sprite_y += (5*diff);
			jump++;
	} else if (jump >= 26 && jump < 52 ) {
		*sprite_y -= (5*diff);
		jump ++;
	} else {
		jump = 0;
		}
	}
	if (stuff) {
	    if (*flag == 1)
	    {
		*sprite_y += 5*diff;
		jump++;
		display(diff, jump, score, dead, 0);
	    }
	    if (*flag == 2)
	    {
		*sprite_y += 5*diff;
		jump++;
		display(diff, jump, score, dead, 1);
	    }
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
void projectImage(float x, float y, float z, GLuint Texture, double diff)
{
	float wid = 30*diff;
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

int checkcollison(int sprite_x, float x, int sprite_y, float y, double diff)
{

	double set = 50;
	if (diff > 1) {
		set *= diff;
	}
	if (x <= sprite_x+set && x >= sprite_x-set) {
		if (y <= sprite_y+set && y >= sprite_y-set) {
			return 1;
	}
	} else {
		return 0; 
	}
	return 0; 
}

void displaybackground(double backgroundx, GLuint Texture, int yres, 
	int xres, bool toggle)
{
	if (!toggle) {
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
	if (x) {
		x-=.0001;
	}
	return x;
}

//updats the background x coordinate and makes it scroll
double backgroundscroll(double x)
{
	if (x) {
		x-=.0006;
	}
	return x;
}

//updats the sky x coordinate and makes it scroll
double sky(double x)
{
	if (x) {
		x-=.00005;
	}
	return x;
}

//updats the grass x coordinate and makes it scroll
double grass(double x)
{
	if (x) {
		x-=.05;
	}
	return x;
}

//updats the ground x coordinate and makes it scroll
double ground(double x)
{
	if (x) {
		x-=.005;
	}
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

void functioncall()
{
	string path = "./images/runner/";
	string filetype = ".png";
	convertImage("runner_sheet2", path, filetype);
	convertImage("money", path, filetype);
	convertImage("life", path, filetype);
	convertImage("jump_sheet", path, filetype);
	convertImage("spear", path, filetype);
	convertImage("saucer", path, filetype);
	convertImage("monster", path, filetype);
	convertImage("runner_sheet2", path, filetype);
	convertImage("runnerdeath_sheet", path, filetype);
	convertImage("bat", path, filetype);

	path = "./images/";
	convertImage("speedboost", path, filetype);
	convertImage("Slide_000", path, filetype);
	convertImage("farbackground", path, filetype);
	convertImage("background1", path, filetype);
	convertImage("grass", path, filetype);
	convertImage("ground", path, filetype);
	convertImage("sky", path, filetype);
	convertImage("game_over", path, filetype);
	convertImage("Smoke", path, filetype);    

}

void deletePPM()
{
	system("rm -f ./images/runner/runner_sheet2.ppm");
	system("rm -f ./images/runner/jump_sheet.ppm");
	system("rm -f ./images/runner/spear.ppm");
	system("rm -f ./images/runner/life.ppm");
	system("rm -f ./images/runner/money.ppm");
	system("rm -f ./images/runner/saucer.ppm");
	system("rm -f ./images/runner/monster.ppm");
	system("rm -f ./images/runner/runner_sheet2.ppm");
	system("rm -f ./images/runner/runnerdeath_sheet.ppm");
	system("rm -f ./images/runner/bat.ppm");

	system("rm -f ./images/speedboost.ppm");
	system("rm -f ./images/Slide_000.ppm");
	system("rm -f ./images/farbackground.ppm");
	system("rm -f ./images/background1.ppm");
	system("rm -f ./images/grass.ppm");
	system("rm -f ./images/ground.ppm");
	system("rm -f ./images/sky.ppm");
	system("rm -f ./images/game_over.ppm");
	system("rm -f ./images/Smoke.ppm");
}

void menuFunc(double xres, double yres, double diff) 
{
	Rect box;
	box.bot = yres/2;
        box.left = (xres/2) - (75*diff);
        box.center = 0;
	ggprint16(&box, 100, 100, "Press Space to Start");
}
