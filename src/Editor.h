#ifndef EDITOR_H
#define EDITOR_H

#include "raylib.h"
#include "raymath.h"

#include "Platform.h"
#include "GameObject.h"

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
    ENTER_LOAD_PATH,
    CONTEXT_MENU,
    SET_EXACT_VALUES
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
    Vector2 buttonsStart = { 30, 30 };  // top left corner of button array
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
    
    bool searchingForSelectedObject = false;
    GameObject* selectedObject = nullptr;
    Vector2 contextMenuPosition;
    float selectedLineThickness = 2.0f;

    const char* contextMenuItems[2] = { "Set Exact Value", "Change Color" };
    int contextMenuItemSelected = -1;
    int contextMenuScrollIndex = 0;
    int contextMenuFocus = -1;

    char exactX[10];
    char exactY[10];
    char exactWidth[10];
    char exactHeight[10];
    bool editingExactValues = false;

    bool savingGameState = false;
    bool loadingGameState = false;
    std::filesystem::path readSavePath();
    std::filesystem::path readLoadPath();
    
    void handleUserInput();
    void draw();
    
    Editor() {
        // set the temporary file paths to something useful on construction
        dummyFilePath.copy(tempSavePath, dummyFilePath.length());
        tempSavePath[dummyFilePath.length() + 1] = '\0';  // null termination
        dummyFilePath.copy(tempLoadPath, dummyFilePath.length());
        tempLoadPath[dummyFilePath.length() + 1] = '\0';  // null termination
    };
};

#endif // EDITOR_H