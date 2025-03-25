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
    
    float horizontalAcceleration = 100.0;
    float horizontalFriction = 100.0;
    float maxHorizontalSpeed = 100.0;

    // increase player gravity when moving upward 
    float upwardGravityMultiplier = 2.0f;

    // pointer to platfrom that the player is on
    // if null the player is "in air"
    Platform *onPlatform = nullptr;
    // could be used for collision detection
    Rectangle getPlayerRectangle();

    void draw();
    void handleUserInput();
    void updatePosition();
    void checkPlatformLanding(Platform *platform);
    void checkWalkOffPlatform();
};


#endif // PLAYER_H