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
    if (IsKeyDown(KEY_SPACE)) {
        if (onPlatform != nullptr) {
            velocity = Vector2Add(velocity, { 0.0, -100.0 });
            onPlatform = nullptr;
        }
    }
}

void Player::updatePosition() {
    position = Vector2Add(position, Vector2Scale(velocity, GetFrameTime()));
    if (onPlatform == nullptr) {
        velocity = Vector2Add(velocity, { 0.0, gravity * GetFrameTime() });
    }
}

void Player::checkPlatformLanding(Platform platform) {
    // check horizontal alignment with platform
    if ((position.x + width > platform.rectangle.x) && (position.x < platform.rectangle.x + platform.rectangle.width)) {
        // check if player will fall into platform and not below platform
        if ((position.y + height + velocity.y * GetFrameTime() > platform.rectangle.y) &&
            (position.y < platform.rectangle.y + platform.rectangle.height)) {
            position.y = platform.rectangle.y - height;
            velocity.y = 0;
            onPlatform = &platform;
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
