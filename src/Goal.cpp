#include "Goal.h"

void Goal::draw() {
    DrawRectangleV(position, { width, height }, color);
}