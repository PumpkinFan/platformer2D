#include "GameState.h"

void GameState::checkToggleDebugInfo() {
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_E)) {
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

    DrawText(debugInformation.str().c_str(), screenWidth - 150, 50, 0, LIME);
}