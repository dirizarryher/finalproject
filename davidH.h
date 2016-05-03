#ifndef DAVIDH_H
#define DAVID_H
#include "Structures.h"
//#include <fmod.h>
extern "C" {
#include "fonts.h"
}

void getRunnerTexture(void);
void setRunnerTexture(Game *g);
void init_sounds(void);
void play_music(int a);
bool endGame(Game *g);
void endMenu(Game *g);
string convertImage(string fn, string p, string ft);



#endif
