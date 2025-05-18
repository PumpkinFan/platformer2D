#include "Player.h"
#include <iostream>

void Player::draw() {
    DrawRectangleV(position, { width, height }, RED);
}

void Player::handleUserInput() {
    // walk left
    if (IsKeyDown(KEY_A) && velocity.x > -maxHorizontalSpeed && !touchingWallLeft) {
        velocity = Vector2Add(velocity, { -horizontalAcceleration * GetFrameTime(), 0.0 });
        if (touchingWallRight) {
            touchingWallRight = nullptr;
        }
    }
    // walk right
    if (IsKeyDown(KEY_D) && velocity.x < maxHorizontalSpeed && !touchingWallRight) {
        velocity = Vector2Add(velocity, { horizontalAcceleration * GetFrameTime(), 0.0 });
        if (touchingWallLeft) {
            touchingWallLeft = nullptr;
        }
    }
    // handle friction
    if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) {
        float frictionModifier = (!onPlatform ? 0.5f : 1.0f);
        if (velocity.x > 0) {
            velocity.x -= horizontalFriction * frictionModifier * GetFrameTime();
        }
        if (velocity.x < 0) {
            velocity.x += horizontalFriction * frictionModifier * GetFrameTime();
        }
        // after reducing the speed if we are near zero just set vx to zero
        if (abs(velocity.x) <= 2) {
            velocity.x = 0;
        }
    }
    if (IsKeyPressed(KEY_SPACE)) {
        // Jumping
        if (onPlatform) {
            velocity = Vector2Add(velocity, { 0.0, -200.0 });
            onPlatform = nullptr;
        }
        else {
            // Wall jumping
            if (touchingWallLeft) {
                velocity = Vector2Add(velocity, { 100, -300 });
                touchingWallLeft = nullptr;
            }
            if (touchingWallRight) {
                velocity = Vector2Add(velocity, { -100, -300 });
                touchingWallRight = nullptr;
            }
        }
    }

    // reset position
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_R)) {
        position = initialPosition;
        velocity = { 0.0, 0.0 };
        onPlatform = nullptr;
        touchingWallLeft = nullptr;
        touchingWallRight = nullptr;
    }
}

void Player::updatePosition() {
    // add velocity
    position = Vector2Add(position, Vector2Scale(velocity, GetFrameTime()));
    // handle gravity
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
    const float platformRightEdge = platform->rectangle.x + platform->rectangle.width;
    // check vertical alignment with platform
    if ((position.y + height > platform->rectangle.y) && (position.y < platform->rectangle.y + platform->rectangle.height)) {
        // check moving into right-edge of platform 
        if ((position.x + velocity.x * GetFrameTime() < platformRightEdge) && 
            (position.x >= platformRightEdge)) {
            position.x = platformRightEdge;
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
    if (touchingWallLeft == nullptr) {
        return;
    }
    if (!(position.y + height > touchingWallLeft->rectangle.y && position.y < touchingWallLeft->rectangle.y + touchingWallLeft->rectangle.height)) {
        touchingWallLeft = nullptr;
    }
}

void Player::checkSlidingOffWallRight() {
    if (touchingWallRight == nullptr) {
        return;
    }
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
