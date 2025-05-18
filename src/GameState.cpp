#include "GameState.h"

void GameState::checkToggleDebugInfo() {
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Q) && !IsKeyDown(KEY_LEFT_SHIFT)) {
        debugInformationOn = !debugInformationOn;
    }
}

void GameState::displayDebugInformation() {
    DrawFPS(GetScreenWidth() - 100, 25);

    std::ostringstream debugInformation;
    debugInformation << "PLAYER X POS " << player.position.x << "\n";
    debugInformation << "PLAYER Y POS " << player.position.y << "\n";
    debugInformation << "PLAYER X VEL " << player.velocity.x << "\n";
    debugInformation << "PLAYER Y VEL " << player.velocity.y << "\n";
    debugInformation << "TOUCHING WALL L " << (bool)(player.touchingWallLeft) << "\n";
    debugInformation << "TOUCHING WALL R " << (bool)(player.touchingWallRight) << "\n";


    DrawText(debugInformation.str().c_str(), screenWidth - 150, 50, 0, LIME);
}