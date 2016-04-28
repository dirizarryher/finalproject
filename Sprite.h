//Name: David A. Hernandez II
//Program: Sprite Header File
//Purpose: 

#ifndef SPRITE_H
#define SPRITE_H
#include <gl\gl.h>

class Sprite
{
    public:
        struct Point
        {
            GLfloat x;
            GLfloat y;
        };
        struct Size
        {
            GLfloat width;
            GLfloat height;
        };
        struct Shape
        {
            GLfloat top;
            GLfloat bottom;
            GLfloat left;
            GLfloat right;
            //GLfloat center??
        };
    protected:
        //Dynamically holds all OPGL texture handles that make up Sprite
        GLuint* textures;
        //Incremented each time a texture is added to Sprite
        unsigned int textureIndex;
        //Incremented each time we want to advance the frame of animation
        unsigned int currentFrame;
        //Stores total number of frames that make up the animation
        unsigned int numberOfFrames;
        //The number of seconds passed before animation frame advances.
        //Helps control speed of the animation
        GLfloat animationDelay;
        //Holds amount of time that elapsed since the last animation
        //frame was changed
        GLfloat animationElapsed;

        //Positions, height & width, velocity of Sprite 
        //larger values of velocity will cause sprite to move
        //more quickly across the screen
        Point position;
        Size size;
        GLfloat velocity;

        //Collision detection flag
        bool collision;
        //Flag to see if sprite should be flipped horizontally
        //when rendering
        bool flipHorizontal;
        //Flag to see if sprite should be flipped vertically
        bool flipVertical;
        //If sprite is currently visible in the game
        bool visible;
        //If sprite is currently active on the screen
        bool active;
        //Tells sprite class to use alpha channel for images that don't have
        //opacity (i.e. background image)
        bool transparent;
        //If sprite is a single texture used to hold the frames of the sprite
        bool isSpriteSheet;
    public:
        //Default Constructor tells Sprite class the number of textures that
        //will be used when Sprite is created so that the correct amount of
        //memory can be allocated for the textures dynamic array
        Sprite(const unsigned int pNumberofTextures);
        //Destructor
        ~Sprite();
        
        //Updates current animation frame and current position of sprite
        void Update(const float pDeltaTime);
        //Actually displays sprite to the screen
        void Render();
        //Adds the required textures to sprite once sprite is created
        const bool AddTexture(const char *pFilename, const bool
                pTransparent = true);

        //Determines which frame of sprite to render when sprite
        //is being rendered. If sprite is a sprite sheet, return textures[0]
        //since there is only one texture at index 0. Otherwise, return
        //texture at index currentFrame. currentFrame is updated in the 
        //sprite Update method.
        const GLuint GetCurrentFrame()
        {
            if(isSpritesheet)
                return textures[0];
            else
                return textures[currentFrame];
        }      

        const Point GetPosition() 
        { 
            return position; 
        }
        const Size GetSize() const 
        { 
            return size; 
        }

        void SetPosition(const GLuint px; const GLfloat py)
        {
            position.x = px;
            position.y = py;
        }
        void SetPosition(const Point pPosition)
        {
            position = pPosition;
        }

        void SetFrameSize(const GLfloat pWidth, const GLfloat pHeight)
        {
            size.width = pWidth;
            size.height = pHeight;
        }
        void SetVelocity(const GLfloat pVelocity)
        {
            velocity = pVelocity;
        }
        void SetNumberOfFrames(const unsigned int pFrames)
        {
            numberOfFrames = pFrames;
        }
        const bool collision() const
        {
            return collision;
        }
        void collision(const bool pValue)
        {
            collision = pValue;
        }
        void flipHorizontal(const bool pValue)
        {
            flipHorizontal = pValue;
        }
        void flipVertical(const bool pValue) 
        {
            flipVertical = pValue;
        }
        void isActive(const bool pValue)
        {
            isActive = pValue;
        }
        const bool isActive() const 
        {
            return isActive;
        }
        void isVisible(const bool pValue)
        {
            isVisible = pValue;
        }
        const bool isVisible() const
        {
            return isVisible;
        }
        void transparent(const bool pValue)
        {
            transparent = pValue;
        }
};

//Allows each sprite to render itself to window screen
void Sprite::Render()
{
    //If isVisible = false, bypass the entire render
    if (isVisible) {
        //Checks if sprite uses transparency. If true, enables transparency
        if (transparent) {
            //Turns on transparency blending
            glEnable(GL_BLEND);
            //Tells OPGL what type of blending to implement
            //AKA allow background images to be seen through transparent
            //sections of the sprite
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        
        //Tells OPGL to render quad
        glBindTexture(GL_TEXTURE_2D, GetCurrentFrame());
        glBegin(GL_QUADS);
        
        //Initializing x & y coordinates for Sprite based on x & y values
        //stored in position. Also need width and height of current frame.
        GLfloat x = position.x;
        GLfloat y = position.y;
        GLfloat w = size.width;
        GLfloat h = size.height;

        //Need to know how much of the texture is going to be rendered
        //In the case of a sprite, only want to render a section of the 
        //texture.

        //Determining width of each frame in the sprite
        GLfloat textWidth = (GLfloat)textureIndex / (GLfloat) numberofFrames;
        GLfloat textHeight = 0.0f;
        //Initializing values for UV Mapping
        GLfloat u = 0.0f;
        GLfloat v = 0.0f;

        if (textureIndex < numberOfFrames) {
            u = (GLfloat)currentFrame *textWidth;
        }

        //Mapping texture of sprite sheet to each corner of the quad
        glTexCoord2f(u, v); glVertex2f(x, y);
        glTexCoord2f(u + textWidth, v); glVertex2f(x + w, y);
        glTexCoord2f(u + textWidth, v + textHeight); glVertex2f(x + w, y + h);
        glTexCoord2f(u, v + textHeight); glVertex2f(x, y + h);

        //Finish rendering
        glEnd();
        
        //Turn off alpha checking once rendering is finished
        if (transparent) {
            glDisable(GL_BLEND);
        }
    }
}

//Update method to Sprite class in order to position the sprite and 
//update the sprite's internal animation relative to computer time
void Sprite::Update(const float pDeltaTime)
{
    float dt = pDeltaTime;
    if (isActive) {
        //Handle the sprite's internal animation
        //Check how much time has passed since last frame change.
        animationElapsed += dt;
        //If elapsed exceeds delay, increment to the next frame
        //This means that the higher the value of delay, the slower the
        //sprite will animate.
        if (animationElapsed >= animationDelay) {
            currentFrame++;
            //If necessary, increment currentFrame to make sure that once
            //total number of frames are exceeded, reset back to 0
            if(currentFrame >= numberOfFrames)
                currentFrame = 0;
            //If frame incremented, also reset elapsed to 0
            animationElapsed = 0.0f;
        }
        //Move sprite based on velocity and dt
        position.x = position.x + velocity * dt;
    }
}
#endif
