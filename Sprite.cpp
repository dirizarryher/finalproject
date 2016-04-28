//Name: David A. Hernandez II
//Program: Sprite source code
//Purpose: 
#include "stdafx.h"
#include "Sprite.h"
#include "SOIL.h"
//SOIL.h to lead sprite sheet properly

//Global variables to hold sprites
//Create Pointers for each sprite needed in the game:
//  Sprite for background
//  Sprite to jump
//  Sprite to slide
//  Sprite to die
Sprite* peonRight;
Sprite* peonRightReel;
Sprite* peonJump;
Sprite* peonSlide;
Sprite* peonDead;
Sprite* background;
Sprite* peon;

const bool LoadTextures() {
    
