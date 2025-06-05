#ifndef PLATFORM_H
#define PLATFORM_H

#include "raylib.h"

struct Platform {
    Rectangle rectangle;
    Color color = GRAY;
    void draw();

    template<class Archive>
    void serialize(Archive& archive) {
        archive(rectangle.x, rectangle.y, rectangle.width, rectangle.height, 
                color.r, color.g, color.b, color.a);
    }
};

#endif // PLATFORM_H