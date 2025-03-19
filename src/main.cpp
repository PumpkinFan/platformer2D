#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "globals.h"
#include "Player.h"


#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Global Game State
//----------------------------------------------------------------------------------
struct GameState {
    Player player;
    Camera2D camera;
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
    game.camera.offset = Vector2 { screenWidth / 2, screenHeight / 2 };
    game.camera.rotation = 0.0f;
    game.camera.zoom = 1.0f;


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
    std::cout << game.player.velocity.x << std::endl;
    // Update
    //----------------------------------------------------------------------------------
    game.player.handleUserInput();
    game.player.updatePosition();

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();



        ClearBackground(RAYWHITE);
        game.player.draw();
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
    //----------------------------------------------------------------------------------
}