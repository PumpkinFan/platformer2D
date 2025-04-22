#ifndef EDITOR_H
#define EDITOR_H

#include "raylib.h"
#include "raymath.h"
#include "Platform.h"

enum EditorMode { 
    DO_NOTHING = 0,
    DRAW_PLATFORM,
    MOVE_PLATFORM
};

class Editor {
public:
    EditorMode mode = DO_NOTHING;
    void setMode(EditorMode newMode);
    // TODO: implement `setMode` function that clears things like selected platform 
    
    bool finishedDrawingPlatform = false; 
    Platform createDrawnPlatform();
    
    bool searchingForSelectedPlatform = false;
    Platform* selectedPlatform = nullptr;
    float selectedLineThickness = 2.0f;
    
    void handleUserInput();
    void draw();
    
private:
    bool drawingPlatform = false;
    Vector2 drawPlatformStart;
    Vector2 drawPlatformEnd;
    Color drawPlatformColor = GRAY;
    
    
};

#endif // EDITOR_H