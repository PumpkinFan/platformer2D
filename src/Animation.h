#include "raylib.h"

// contains necessary info to use a sprite sheet for animation
// expects sprite sheets to be laid out horizontally from left to right
struct Animation {
    int firstFrameIndex;
    int lastFrameIndex;
    
    int offsetSize;
    int bufferSize;
    
    float frameWidth;
    float frameHeight; 
    
    float frameDuration = 0.1;

    int currentFrameIndex = 0;
    float currentFrameDuration = 0.0;
    
    int frameIncrement = 1;
    
    void update();
    Rectangle getCurrentFrame();
};