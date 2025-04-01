#include <iostream>
#include <vector>

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "globals.h"
#include "Player.h"
#include "Editor.h"


#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

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


struct GameState {
    Player player;
    Camera2D camera;
    Editor editor;
    std::vector<Platform> platforms;
    void addNewPlatform(Platform newPlatform) {
        platforms.push_back(newPlatform);
    };
};


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

    // Initialize the camera
    game.camera.target = Vector2 { game.player.position.x + game.player.width / 2, 
                                   game.player.position.y + game.player.height / 2 };
    game.camera.offset = Vector2 { screenWidth / 2, screenHeight / 2 };
    game.camera.rotation = 0.0f;
    game.camera.zoom = 1.0f;

    game.platforms = createPlatforms();

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

    return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    if (!game.player.touchingWallLeft) {
        for (Platform& platform : game.platforms) {
            game.player.checkCollidingWallLeft(&platform);
        }
    }
    else {
        game.player.checkSlidingOffWallLeft();
    }
    if (!game.player.touchingWallRight) {
        for (Platform& platform : game.platforms) {
            game.player.checkCollidingWallRight(&platform);
        }
    }
    else {
        game.player.checkSlidingOffWallRight();
    }
    if (!game.player.onPlatform) {
        for (Platform& platform : game.platforms) {
            game.player.checkPlatformLanding(&platform);
        }
    }
    else {
        game.player.checkWalkOffPlatform();
    }
    
    game.player.handleUserInput();
    game.player.updatePosition();

    game.editor.handleUserInput();
    if (game.editor.finishedDrawingPlatform) {
        game.addNewPlatform(game.editor.createDrawnPlatform());
        game.editor.finishedDrawingPlatform = false;
    } 
    // std::cout << game.platforms.size() << std::endl;

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    
        ClearBackground(RAYWHITE);
        
        BeginMode2D(game.camera);
            for (Platform platform : game.platforms) {
                platform.draw();
            }
            game.player.draw();

            if (game.editor.isActive) {
                // std::cout << game.editor.mode << std::endl;
                if (GuiButton((Rectangle){ 30, 30, 30, 30 }, "#33#")) {
                    if (game.editor.mode != DRAW_PLATFORM) {
                        game.editor.mode = DRAW_PLATFORM;
                    }
                };

                game.editor.draw();
            }

        EndMode2D();

    EndDrawing();
    //----------------------------------------------------------------------------------
}