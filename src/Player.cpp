#include "Player.h"
#include <iostream>

void Player::draw() {
    // running to the left
    if (IsKeyDown(KEY_A)) {
        Rectangle currentFrame = runningAnimation.getCurrentFrame();
        currentFrame.width = currentFrame.width * -1.0f;
        DrawTexturePro(runningTexture, currentFrame, Rectangle { position.x, position.y, runningAnimation.frameWidth, runningAnimation.frameHeight },
        Vector2 { 0.0, 0.0 }, 0.0, WHITE);
    }
    // running to the right
    else if (IsKeyDown(KEY_D)) {
        DrawTexturePro(runningTexture, runningAnimation.getCurrentFrame(), Rectangle { position.x, position.y, runningAnimation.frameWidth, runningAnimation.frameHeight },
        Vector2 { 0.0, 0.0 }, 0.0, WHITE);
    }
    // idle
    else {
        DrawTexture(idleTexture, position.x, position.y, WHITE);
    }
}

void Player::resetPosition() {
    position = initialPosition;
    velocity = { 0.0, 0.0 };
    onPlatform = nullptr;
    touchingWallLeft = nullptr;
    touchingWallRight = nullptr;
    reachedGoal = nullptr;
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
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_R)) {
        resetPosition();
    }
}

void Player::updatePosition() {
    // guard clause if player has finished the level
    if (reachedGoal) {
        const Vector2 targetPosition = {reachedGoal->position.x + 0.5f * reachedGoal->width - 0.5f * width,
                                        reachedGoal->position.y + 0.5f * reachedGoal->height - 0.5F * height};
        const Vector2 goalAnimationVelocity = Vector2Scale(Vector2Subtract(targetPosition, position), 2.0);                                
        if (!(Vector2Equals(position, targetPosition))) {
            position = Vector2Add(position, Vector2Scale(goalAnimationVelocity, GetFrameTime()));
        }
        return;
    }
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
        if ((position.x < platformRightEdge) && 
            (position.x + width >= platformRightEdge)) {
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
        if ((position.x + width > platform->rectangle.x) && 
            (position.x <= platform->rectangle.x)) {
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

void Player::checkCollisionWithGoal(Goal *goal) {
    if (CheckCollisionRecs(getPlayerRectangle(), Rectangle { goal->position.x, goal->position.y, goal->width, goal->height })) {
        reachedGoal = goal;
    }
}

Rectangle Player::getPlayerRectangle() {
    return Rectangle { position.x, position.y, width, height };
}
