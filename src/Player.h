#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "raymath.h"
#include "globals.h"

struct Player {
    Vector2 position = { screenWidth / 2, screenHeight / 2 };
    Vector2 velocity = { 50.0, 0.0 };
    float width = 10.0;
    float height = 10.0;
    void draw();
    void handleUserInput();
    void updatePosition();
};


#endif // PLAYER_H