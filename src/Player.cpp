#include "Player.h"
#include <iostream>

void Player::draw() {
    DrawRectangleV(position, { width, height }, RED);
}

void Player::handleUserInput() {
    if (IsKeyDown(KEY_A) && velocity.x > -maxHorizontalSpeed && !touchingWallLeft) {
        velocity = Vector2Add(velocity, { -horizontalAcceleration * GetFrameTime(), 0.0 });
        if (touchingWallRight) {
            touchingWallRight = nullptr;
        }
    }
    if (IsKeyDown(KEY_D) && velocity.x < maxHorizontalSpeed && !touchingWallRight) {
        velocity = Vector2Add(velocity, { horizontalAcceleration * GetFrameTime(), 0.0 });
        if (touchingWallLeft) {
            touchingWallLeft = nullptr;
        }
    }
    if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) {
        if (velocity.x > 0) {
            velocity.x -= horizontalFriction * GetFrameTime();
        }
        if (velocity.x < 0) {
            velocity.x += horizontalFriction * GetFrameTime();
        }
        // after reducing the speed if we are near zero just set vx to zero
        if (abs(velocity.x) < 1) {
            velocity.x = 0;
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

void Player::checkCollidingWallLeft(Platform *platform) {
    // check vertical alignment with platform
    if ((position.y + height > platform->rectangle.y) && (position.y < platform->rectangle.y + platform->rectangle.height)) {
        // check moving into right-edge of platform 
        if ((position.x + velocity.x * GetFrameTime() < platform->rectangle.x + platform->rectangle.width) && 
            (position.x >= platform->rectangle.x + platform->rectangle.width)) {
            position.x = platform->rectangle.x + platform->rectangle.width;
            velocity.x = 0;
            touchingWallLeft = platform;
        }
    }
}

void Player::checkCollidingWallRight(Platform *platform) {
    // check vertical alignment with platform
    if ((position.y + height > platform->rectangle.y) && (position.y < platform->rectangle.y + platform->rectangle.height)) {
        // check moving into left-edge of platform or at the left-edge
        if ((position.x + velocity.x * GetFrameTime() + width > platform->rectangle.x) && 
            (position.x + width <= platform->rectangle.x)) {
            position.x = platform->rectangle.x - width;
            velocity.x = 0;
            touchingWallRight = platform;
        }
    }
}

void Player::checkSlidingOffWallLeft() {
    if (!(position.y + height > touchingWallLeft->rectangle.y && position.y < touchingWallLeft->rectangle.y + touchingWallLeft->rectangle.height)) {
        touchingWallLeft = nullptr;
    }
}

void Player::checkSlidingOffWallRight() {
    if (!(position.y + height > touchingWallRight->rectangle.y && position.y < touchingWallRight->rectangle.y + touchingWallRight->rectangle.height)) {
        touchingWallRight = nullptr;
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
