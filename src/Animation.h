#include "raylib.h"

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