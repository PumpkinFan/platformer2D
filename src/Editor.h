#ifndef EDITOR_H
#define EDITOR_H

#include "raylib.h"
#include "raymath.h"
#include "Platform.h"

enum EditorMode { 
    DO_NOTHING = 0,
    DRAW_PLATFORM
};

class Editor {
public:
    bool isActive = false;
    EditorMode mode = DO_NOTHING;
    
    bool drawingPlatform = false;
    Vector2 drawPlatformStart;
    Vector2 drawPlatformEnd;
    Color drawPlatformColor = GRAY;
    bool finishedDrawingPlatform = false;
    Platform createDrawnPlatform();

    void handleUserInput();
    void draw();

private:


};

#endif // EDITOR_H