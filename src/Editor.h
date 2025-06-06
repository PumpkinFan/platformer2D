#ifndef EDITOR_H
#define EDITOR_H

#include "raylib.h"
#include "raymath.h"

#include "Platform.h"

#include <vector>
#include <functional>
#include <string>
#include <cstring>
#include <filesystem>

enum EditorMode { 
    DO_NOTHING = 0,
    DRAW_PLATFORM,
    SELECT_PLATFORM,
    ENTER_SAVE_PATH,
    ENTER_LOAD_PATH
};

// Custom button struct. `onClick` triggers when button is left-clicked.
struct EditorButton {
    Rectangle bounds;
    const char* text;
    // bool* selected;
    std::function<void(void)> onClick;

    void draw();
};

class Editor {
private:
    float buttonSize = 30;
    float buttonPadding = 10;
    Vector2 buttonsStart = { 30, 30 };
    std::vector<EditorButton> generateButtons();  
    std::vector<EditorButton> buttons = generateButtons();
    
    bool drawingPlatform = false;
    Vector2 drawPlatformStart;
    Vector2 drawPlatformEnd;
    Color drawPlatformColor = GRAY;

    std::string dummyFilePath = std::filesystem::current_path().append("gamestate.bin").string();
    char tempSavePath[128];
    char tempLoadPath[128];
    std::filesystem::path saveTargetPath;
    std::filesystem::path loadTargetPath;
    
public:
    bool isActive = false;
    void checkActiveToggle();
    
    EditorMode mode = DO_NOTHING;
    void setMode(EditorMode newMode);
    
    Platform createDrawnPlatform();
    bool finishedDrawingPlatform = false; 
    
    bool searchingForSelectedPlatform = false;
    Platform* selectedPlatform = nullptr;
    float selectedLineThickness = 2.0f;

    bool savingGameState = false;
    bool loadingGameState = false;
    std::filesystem::path readSavePath();
    std::filesystem::path readLoadPath();
    
    void handleUserInput();
    void draw();
    
    // set the temporary file paths to something useful on construction
    Editor() {
        dummyFilePath.copy(tempSavePath, dummyFilePath.length());
        tempSavePath[dummyFilePath.length() + 1] = '\0';  // null termination
        dummyFilePath.copy(tempLoadPath, dummyFilePath.length());
        tempLoadPath[dummyFilePath.length() + 1] = '\0';  // null termination
    };
};

#endif // EDITOR_H