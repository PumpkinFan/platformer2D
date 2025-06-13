#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "raymath.h"
#include "globals.h"
#include "Platform.h"
#include "Goal.h"

struct Player {
private:
    const char *textureFilePath = "assets/stickman32.png";
    Texture2D texture;
    
public:
    Vector2 initialPosition = { 400, 225 };
    Vector2 position = initialPosition;
    Vector2 velocity = { 0.0, 0.0 };
    float width = 32.0;
    float height = 32.0;
    
    float horizontalAcceleration = 200.0;
    float horizontalFriction = 200.0;
    float maxHorizontalSpeed = 200.0;

    // increase player gravity when moving upward 
    float upwardGravityMultiplier = 2.0f;

    // pointer to platfrom that the player is on
    // if null the player is "in air"
    Platform *onPlatform = nullptr;

    // point to platform touched by player on the left
    // if null the player isn't touching anything to the left
    Platform *touchingWallLeft = nullptr;
    
    // point to platform touched by player on the right
    // if null the player isn't touching anything to the right
    Platform *touchingWallRight = nullptr;
    
    // could be used for collision detection
    Rectangle getPlayerRectangle();

    void draw();
    void handleUserInput();
    void updatePosition();
    void checkPlatformLanding(Platform *platform);
    void checkWalkOffPlatform();
    void checkCollidingWallLeft(Platform *platform);
    void checkCollidingWallRight(Platform *platform);
    void checkSlidingOffWallLeft();
    void checkSlidingOffWallRight();

    void checkCollisionWithGoal(Goal *goal);
    // pointer to a goal the player has reached
    // if null the player is still completing the level
    Goal *reachedGoal = nullptr;

    template<class Archive>
    void serialize(Archive& archive) {
        archive(
            position.x,
            position.y,
            initialPosition.x,
            initialPosition.y,
            velocity.x,
            velocity.y
        );
    }
    void loadTexture() {
        texture = LoadTexture(textureFilePath);
    }
    void unloadTexture() {
        UnloadTexture(texture);
    }

};


#endif // PLAYER_H