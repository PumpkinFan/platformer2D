#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>

#include "raylib.h"

#include "cereal/types/vector.hpp"
#include "cereal/archives/binary.hpp"

#include "globals.h"
#include "Player.h"
#include "Editor.h"



struct GameState {
    Player player;
    Camera2D camera;
    Editor editor;
    std::vector<Platform> platforms;
    void addNewPlatform(Platform newPlatform) {
        platforms.push_back(newPlatform);
    };

    void checkToggleDebugInfo();
    bool debugInformationOn = false;
    void displayDebugInformation();

    // TODO: Figure out how to ignore this on Web
    void saveGameState(std::filesystem::path savePath){
        std::ofstream outputStream(savePath, std::ios::binary);
        cereal::BinaryOutputArchive archive(outputStream);

        archive(platforms);
        outputStream.close();
    };
    void loadGameState(std::filesystem::path loadPath) {
        std::ifstream inputStream(loadPath, std::ios::binary);
        cereal::BinaryInputArchive archive(inputStream);

        archive(platforms);
        inputStream.close();
    };
};

#endif
