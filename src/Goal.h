#ifndef GOAL_H
#define GOAL_H

#include "raylib.h"

struct Goal {
    Vector2 position = { 300, 250 };
    float width = 50;
    float height = 50;
    Color color = SKYBLUE;
    void draw();

};

#endif // GOAL_H