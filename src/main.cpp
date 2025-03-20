#include <iostream>
#include <vector>
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

std::vector<Platform> createPlatforms() {
    std::vector<Platform> platforms;
    Platform platform = { Rectangle { 100, 400, 700, 100 } };
    platforms.push_back(platform);
    return platforms;
}

struct GameState {
    Player player;
    Camera2D camera;
    std::vector<Platform> platforms;
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
    game.camera.target = Vector2 { game.player.position.x + 100, 
                                   game.player.position.y + 100 };
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
    for (Platform platform : game.platforms) {
        game.player.checkPlatformLanding(platform);
    }
    game.player.handleUserInput();
    game.player.updatePosition();

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    
        ClearBackground(RAYWHITE);
        
        BeginMode2D(game.camera);
            for (Platform platform : game.platforms) {
                platform.draw();
            }
            game.player.draw();
        EndMode2D();

    EndDrawing();
    //----------------------------------------------------------------------------------
}