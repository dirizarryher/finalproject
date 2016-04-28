#include "Sprite.h"
#include "SOIL.h"

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
    isVisible = false;
    isActive = false;
    isSpriteSheet = false;
}

Sprite::~Sprite()
{
    delete [] textures;
}



