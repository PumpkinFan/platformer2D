#ifndef GOAL_H
#define GOAL_H

#include "raylib.h"

struct Goal {
    Rectangle hitbox;
    Color color = SKYBLUE;
    void draw();

};

#endif // GOAL_H