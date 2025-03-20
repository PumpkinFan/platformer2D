#ifndef PLATFORM_H
#define PLATFORM_H

#include "raylib.h"

struct Platform {
    Rectangle rectangle;
    Color color = GRAY;
    void draw();
};

#endif