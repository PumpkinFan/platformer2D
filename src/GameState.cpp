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
    debugInformation << "REACHED GOAL " << (bool)(player.reachedGoal) << "\n";

    DrawText(debugInformation.str().c_str(), screenWidth - 150, 50, 0, LIME);
}

void GameState::saveGameState(std::filesystem::path savePath) {
    std::ofstream outputStream(savePath, std::ios::binary);
    cereal::BinaryOutputArchive archive(outputStream);

    archive(platforms);
    archive(player);
    // archive(editor);
    outputStream.close();
};

void GameState::loadGameState(std::filesystem::path loadPath) {
    std::ifstream inputStream(loadPath, std::ios::binary);
    if (inputStream) {
        cereal::BinaryInputArchive archive(inputStream);
        archive(platforms);
        archive(player);
    }
    else {
        std::cerr << "Failed to open file at " << loadPath.string() << "\n";
    }

    inputStream.close();
};

void GameState::loadLevel(Level level) {
    loadGameState(level.startingGameStatePath);
    player.initialPosition = level.initialPlayerPosition;
    player.position = level.initialPlayerPosition;
}
