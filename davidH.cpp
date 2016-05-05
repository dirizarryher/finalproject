// David A. Hernandez II CS335
#include "davidH.h"
#include "Structures.h"
#include <cstdio>
//#include <FMOD/fmod.h>
//#include <FMOD/wincompat.h>
//#include <fmod.h>
extern "C" {
#include "fonts.h"
}

Ppmimage *idleImage = NULL;
Ppmimage *runningImage = NULL;
Ppmimage *jumpImage = NULL;
Ppmimage *deathImage = NULL;
GLuint idleTexture, runnerTexture, jumpTexture, deathTexture;
GLuint idleSilhouetteTexture, runnerSilhouetteTexture, 
       jumpSilhouetteTexture, deathSilhouetteTexture;

void getRunnerTexture(void)
{
    //Loading the image files into a PPM structure
    idleImage = ppm6GetImage("./images/runner/idle_sheet.ppm");
    runningImage = ppm6GetImage("./images/runner/runner_sheet2.ppm");
    jumpImage = ppm6GetImage("./images/runner/jump_sheet.ppm");
    deathImage = ppm6GetImage("./images/runner/runner_sheet2.ppm");

    //Creating OPGL texture elements for runner when he's idle, 
    //running, jumping, or when he dies
    glGenTextures(1, &idleTexture);
    glGenTextures(1, &runnerTexture);
    glGenTextures(1, &jumpTexture);
    glGenTextures(1, &deathTexture);

//------------------------------------------------------------------------
//idleTexture
    //TextureImage for runner when idle
    glBindTexture(GL_TEXTURE_2D, idleTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, idleImage->width, idleImage->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, idleImage->data);

/*    //Silhouettetexture for when runner is idle
    glBindTexture(GL_TEXTURE_2D, idleSilhouetteTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char *idleSilhouetteData = buildAlphaData(idleImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, idleImage->width, 
            idleImage->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
            idleSilhouetteData);
*/

//------------------------------------------------------------------------
//runnerTexture
    //TextureImage for runner when running
    glBindTexture(GL_TEXTURE_2D, runnerTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, runningImage->width, 
            runningImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
            runningImage->data);

/*    //Silhouettetexture for Running
    glBindTexture(GL_TEXTURE_2D, runnerSilhouetteTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char *runnerSilhouetteData = buildAlphaData(runningImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, runningImage->width,
            runningImage->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
            runnerSilhouetteData);
*/

//------------------------------------------------------------------------
//jumpTexture
    //TextureImage when Runner jumps
    glBindTexture(GL_TEXTURE_2D, jumpTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, jumpImage->width, jumpImage->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, jumpImage->data);

    //Silhouettetexture for jumping
    glBindTexture(GL_TEXTURE_2D, jumpSilhouetteTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char *jumpSilhouetteData = buildAlphaData(jumpImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, jumpImage->width,
            jumpImage->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
            jumpSilhouetteData);


//------------------------------------------------------------------------
    //SilhouetteTexture for dying
    glBindTexture(GL_TEXTURE_2D, deathTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //TextureImage when Runner dies
    glBindTexture(GL_TEXTURE_2D, deathSilhouetteTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, deathImage->width, deathImage->height,
            0, GL_RGB, GL_UNSIGNED_BYTE, deathImage->data);

    unsigned char *deathSilhouetteData = buildAlphaData(deathImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, deathImage->width,
            deathImage->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
            deathSilhouetteData);
}

void showMyName()
{
    Rect r;
    
    glColor3ub(255, 0, 255);
    glPushMatrix();
    glTranslatef(350, 400, 0);
    float w = 200;
    float h = 100;
    r.left = w;
    r.bot = h;
    glBegin(GL_QUADS);
    glVertex2i(-w,-h);
    glVertex2i(-w, h);
    glVertex2i( w, h);
    glVertex2i( w,-h);
    glEnd();
    ggprint8b(&r, 32, 0x00ff0000, "David A. Hernandez II");
    glPopMatrix();

}

/*
   void init_sounds(void)
   {
   if (fmod_init()) {
   printf("ERROR");
   return;
   }
   if (fmod_createsound((char *)"./sounds/untitled.mp3", 0)) {
   printf("ERROR");
   return;
   }
   if (fmod_createsound((char *)"./sounds/untitled2.mp3", 1)) {
   printf("ERROR");
   return;
   }
   fmod_setmode(0, FMOD_LOOP_NORMAL);
   fmod_setmode(1, FMOD_LOOP_NORMAL);
   }

   void play_music(int a)
   {
   fmod_playsound(a);
   }


//Converts an image with any file extension imported into 
//images/runner/ directory into a ppm image
string convertImage(string filename, string path, string filetype)
{
    string oldfile = path + filename + filetype;
    string newfile = path + filename + ".ppm";

    //convert ./images/runner/runner_sheet2.ppm ./images/runner/runner_sheet2.png
    string newstuff =  "convert " + oldfile + " " + newfile;
    filename = system(newstuff.data());
    return filename;
}

//Boolean value to determine if runner touched an obstacle
bool endGame(Game *g)
{
    //If runner dies, game over
    if (runner.dead == 1)
        return true;
    else
        return false;
}
*/

//Game Over Menu if runner dies
void endMenu(Game *g)
{
    int yellow = 0x00ffff00;
    int red = 0x00ff0000;

    Rect r;
    r.bot = yres - 300;
    r.left = 600;
    ggprint16(&r, 50, red, "GAME OVER");
    ggprint16(&r, 50, red, "You died!");
    ggprint16(&r, 50, yellow, "Your Score: %i", g->score);
    ggprint16(&r, 50, yellow, "Your Time: %i seconds", g->gameTimer); 

    r.bot = yres - 600;
    ggprint16(&r, 50, yellow, "Press ESC to Exit");
}

