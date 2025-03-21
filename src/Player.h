#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "raymath.h"
#include "globals.h"
#include "Platform.h"

struct Player {
    Vector2 position = { screenWidth / 2, screenHeight / 2 };
    Vector2 velocity = { 0.0, 0.0 };
    float width = 10.0;
    float height = 10.0;

    // pointer to platfrom that the player is on
    Platform *onPlatform = nullptr;
    // could be used for collision detection
    Rectangle getPlayerRectangle();

    void draw();
    void handleUserInput();
    void updatePosition();
    void checkPlatformLanding(Platform platform);
    void checkWalkOffPlatform();
};


#endif // PLAYER_H