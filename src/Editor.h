#ifndef EDITOR_H
#define EDITOR_H

#include "raylib.h"
#include "raymath.h"
#include "Platform.h"

#include <vector>
#include <functional>

enum EditorMode { 
    DO_NOTHING = 0,
    DRAW_PLATFORM,
    SELECT_PLATFORM
};

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
    
    void handleUserInput();
    void draw();
    
    
};

#endif // EDITOR_H