#include "Player.h"
#include <iostream>

void Player::draw() {
    DrawRectangleV(position, { width, height }, RED);
}

void Player::handleUserInput() {
    // keyboard movement input
    if (abs(velocity.x) < maxHorizontalSpeed) {
        if (IsKeyDown(KEY_A)) {
            velocity = Vector2Add(velocity, { -horizontalAcceleration * GetFrameTime(), 0.0 });
        }
        if (IsKeyDown(KEY_D)) {
            velocity = Vector2Add(velocity, { horizontalAcceleration * GetFrameTime(), 0.0 });
        }
    }
    if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) {
        if (velocity.x > 0) {
            velocity.x -= horizontalFriction * GetFrameTime();
        }
        if (velocity.x < 0) {
            velocity.x += horizontalFriction * GetFrameTime();
        }
    }
    if (IsKeyDown(KEY_SPACE)) {
        // Jumping
        if (onPlatform != nullptr) {
            velocity = Vector2Add(velocity, { 0.0, -200.0 });
            onPlatform = nullptr;
        }
    }
}

void Player::updatePosition() {
    position = Vector2Add(position, Vector2Scale(velocity, GetFrameTime()));
    if (onPlatform == nullptr) {
        if (velocity.y < 0) {  // player moving upwards
            velocity = Vector2Add(velocity, { 0.0, gravity * upwardGravityMultiplier * GetFrameTime() });
        }
        else {
            velocity = Vector2Add(velocity, { 0.0, gravity * GetFrameTime() });
        }
    }
}

void Player::checkPlatformLanding(Platform *platform) {
    // check horizontal alignment with platform
    if ((position.x + width > platform->rectangle.x) && (position.x < platform->rectangle.x + platform->rectangle.width)) {
        // check if player will fall into platform and not below platform
        if ((position.y + height + velocity.y * GetFrameTime() > platform->rectangle.y) &&
            (position.y + height < platform->rectangle.y)) {
            position.y = platform->rectangle.y - height;
            velocity.y = 0;
            onPlatform = platform;
        }
    }
}

void Player::checkWalkOffPlatform() {
    if (onPlatform == nullptr) {
        return;
    }
    if ((position.x + width < onPlatform->rectangle.x) || 
        (position.x > onPlatform->rectangle.x + onPlatform->rectangle.width)) {
        onPlatform = nullptr;
    }
}

Rectangle Player::getPlayerRectangle() {
    return Rectangle { position.x, position.y, width, height };
}
