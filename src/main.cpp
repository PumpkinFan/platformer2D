#include <iostream>
#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "globals.h"
#include "GameState.h"
#include "Level.h"


#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

// TODO
// New editor features (specify exact values, align platforms, ...)
// Pixel art - design player character, platforms, game objects, etc.
// New game mechanics (coins, end goal)
// Fix animation bug when player is moving left


//----------------------------------------------------------------------------------
// Global Game State
//----------------------------------------------------------------------------------

std::vector<Platform> createPlatforms() {
    std::vector<Platform> platforms;
    Platform platform1 = { Rectangle { 150, 400, 500, 50 } };
    Platform platform2 = { Rectangle { 300, 300, 100, 10 } };
    Platform platform3 = { Rectangle { 375, 350, 100, 10 } };
    Platform verticalPlatformLeft = { Rectangle { 100, 100, 50, 300 } };
    Platform verticalPlatformRight = { Rectangle { 650, 100, 50, 300 } };
    platforms.push_back(platform1);
    platforms.push_back(platform2);
    platforms.push_back(platform3);
    platforms.push_back(verticalPlatformLeft);
    platforms.push_back(verticalPlatformRight);
    return platforms;
}

// start on first level, incremented whenever player reaches goal
unsigned int currentLevel = 0;
std::vector<Level> levels = { 
    Level { "gray plats", { 400, 225 }, "levels/gray_plats.bin" },
    Level { "green plats", { 500, 50 }, "levels/green_plats.bin" } 
};
// keep track of the amount of time since reaching the goal
float nextLevelTimer = 0;
// wait this amount of time before advancing the level after player reaches goal
float nextLevelThreshold = 2.0;

GameState game;

//----------------------------------------------------------------------------------
// Module functions declaration
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void);     // Update and Draw one frame

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "platformer2D");

    
    // Load player texture
    game.player.loadTexture();
    
    // Initialize the camera
    game.camera.target = Vector2 { 0, 0 };
    game.camera.offset = Vector2 { 0, 0 };
    game.camera.rotation = 0.0f;
    game.camera.zoom = 1.0f;
    
    game.loadLevel(levels[currentLevel]);
    // game.platforms = createPlatforms();


#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    // Free any loaded assets
    //--------------------------------------------------------------------------------------
    game.player.unloadTexture();
    return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    
    // TODO: refactor this for performance
    // Player mechanics
    //----------------------------------------------------------------------------------
    if (!game.player.touchingWallLeft) {
        for (Platform& platform : game.platforms) {
            game.player.checkCollidingWallLeft(&platform);
            if (game.player.touchingWallLeft) {
                break;
            }
        }
    }
    else {
        game.player.checkSlidingOffWallLeft();
    }
    if (!game.player.touchingWallRight) {
        for (Platform& platform : game.platforms) {
            game.player.checkCollidingWallRight(&platform);
            if (game.player.touchingWallRight) {
                break;
            }
        }
    }
    else {
        game.player.checkSlidingOffWallRight();
    }
    if (!game.player.onPlatform) {
        for (Platform& platform : game.platforms) {
            game.player.checkPlatformLanding(&platform);
            if (game.player.onPlatform) {
                break;
            }
        }
    }
    else {
        game.player.checkWalkOffPlatform();
    }

    game.player.handleUserInput();
    game.player.checkCollisionWithGoal(&game.goal);
    game.player.updatePosition();
    game.player.updateAnimation();    

    if (game.player.reachedGoal && (nextLevelTimer < nextLevelThreshold)) {
        nextLevelTimer += GetFrameTime();
        if (nextLevelTimer >= nextLevelThreshold && currentLevel < levels.size() - 1) {
            currentLevel += 1;
            game.loadLevel(levels[currentLevel]);
            nextLevelTimer = 0;
        }
    }
    
    // Editor stuff
    //----------------------------------------------------------------------------------
    game.editor.checkActiveToggle();

    // Check user input and update state of editor 
    if (game.editor.isActive) {
        game.editor.handleUserInput();
        if (game.editor.finishedDrawingPlatform) {
            game.addNewPlatform(game.editor.createDrawnPlatform());
            game.editor.finishedDrawingPlatform = false;
        } 
        if (game.editor.searchingForSelectedPlatform) {
            for (Platform& platform : game.platforms) {
                if (CheckCollisionPointRec(GetMousePosition(), platform.rectangle)) {
                    game.editor.selectedPlatform = &platform;
                    game.editor.searchingForSelectedPlatform = false;
                    break;
                }
            }
        }
    }

    
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    
    ClearBackground(RAYWHITE);
    
    BeginMode2D(game.camera);
    for (Platform platform : game.platforms) {
        platform.draw();
    }
    game.goal.draw();
    game.player.draw();
    
    if (game.editor.isActive) {
        game.editor.draw();
    }
    
    EndMode2D();

    // Display debug information
    game.checkToggleDebugInfo();
    if (game.debugInformationOn) {
        game.displayDebugInformation();
    }
    
    EndDrawing();
    //----------------------------------------------------------------------------------

    // Check if we are saving or loading the game state
    if (game.editor.savingGameState) {
        game.saveGameState(game.editor.readSavePath());
    }
    if (game.editor.loadingGameState) {
        game.loadGameState(game.editor.readLoadPath());
    }

}
