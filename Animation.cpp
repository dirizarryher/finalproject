//Name: David A. Hernandez II
//Program: Sprite source code
//Purpose: 
#include "Sprite.h"

//Global variables to hold sprites
//Create Pointers for each sprite needed in the game:
//  Sprite for background
//  Sprite to jump
//  Sprite to slide
//  Sprite to die
Sprite* peonRightReel;
Sprite* peonJumpReel;
Sprite* peonSlideReel;
Sprite* peonDeadReel;
Sprite* background;
Sprite* player;
//Created pointers for each sprite:
    //Moving peon right
    //Making peon jump
    //Making peon slide
    //A sprite to sequence dying
    //A sprite for the background



const bool LoadTextures() 
{
    background = new Sprite(1);
    background->SetFrameSize(1877.0f, 600.f);
    background->SetNumberOfFrames(1);
    background->AddTexture("<filename>", false);

    peonRightReel = new Sprite(1);
    peonRightReel->SetFramesize(125.0f, 100.f);
    peonRightReel->SetNumberOfFrames(<number>);
    peonRightReel->SetPosition(0, screen_height - 130.0f);
    peonRightReel->AddTexture("<filename>",);

