#pragma once

#include "raylib.h"

class GameObject {
public:
    virtual ~GameObject() = default;
    virtual Rectangle getRect() const = 0;
    virtual void setRect(Rectangle rect) = 0;
};
