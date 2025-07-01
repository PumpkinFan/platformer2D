#include "Animation.h"

void Animation::update() {
    float dt = GetFrameTime();
    currentFrameDuration += dt;

    if (currentFrameDuration >= frameDuration) {
        currentFrameDuration = 0.0;
        currentFrameIndex += frameIncrement;

        // boomerang-type looping
        if (currentFrameIndex > lastFrameIndex) {
            currentFrameIndex = lastFrameIndex - 1;
            frameIncrement = -1;
        }
        if (currentFrameIndex < firstFrameIndex) {
            currentFrameIndex = firstFrameIndex + 1;
            frameIncrement = +1;
        }
    }
}

Rectangle Animation::getCurrentFrame() {
    float x = offsetSize +  currentFrameIndex * (frameWidth + bufferSize);

    return Rectangle { x, 0, frameWidth, frameHeight };  
}
