#ifndef GOAL_H
#define GOAL_H

#include "raylib.h"
#include "GameObject.h"

struct Goal : public GameObject {
    Vector2 position = { 300, 250 };
    float width = 50;
    float height = 50;
    Color color = SKYBLUE;
    void draw();

    Rectangle getRect() const override {
        return { position.x, position.y, width, height };
    }

    void setRect(Rectangle rect) override {
        position.x = rect.x;
        position.y = rect.y;
        width = rect.width;
        height = rect.height;
    }
};

#endif // GOAL_H
