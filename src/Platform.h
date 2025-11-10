#ifndef PLATFORM_H
#define PLATFORM_H

#include "raylib.h"
#include "GameObject.h"

struct Platform : public GameObject {
    Rectangle rectangle;
    Color color = GRAY;
    void draw();

    Rectangle getRect() const override {
        return rectangle;
    }

    void setRect(Rectangle rect) override {
        rectangle = rect;
    }

    template<class Archive>
    void serialize(Archive& archive) {
        archive(rectangle.x, rectangle.y, rectangle.width, rectangle.height, 
                color.r, color.g, color.b, color.a);
    }
};

#endif // PLATFORM_H
