#include "Player.h"

void Player::draw() {
    DrawRectangleV(position, { width, height }, RED);
}


void Player::handleUserInput() {
    // keyboard movement input
    if (IsKeyDown(KEY_A)) {
        velocity = Vector2Add(velocity, { -1.0, 0.0 });
    }
    if (IsKeyDown(KEY_D)) {
        velocity = Vector2Add(velocity, { 1.0, 0.0 });
    }
}


void Player::updatePosition() {
    position = Vector2Add(position, Vector2Scale(velocity, GetFrameTime()));
    velocity = Vector2Add(velocity, { 0.0, gravity * GetFrameTime() });
}
