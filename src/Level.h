#ifndef LEVEL_H
#define LEVEL_H

#include <filesystem>
#include <string>

#include "raylib.h"

#include "Player.h"
#include "Platform.h"

// contains level information such as level label, player start position,
// and path to load starting gamestate
struct Level
{
    std::string label;
    Vector2 initialPlayerPosition;
    std::filesystem::path startingGameStatePath;
};

#endif // LEVEL_H
