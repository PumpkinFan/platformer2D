#include "Editor.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

void Editor::setMode(EditorMode newMode) {
    if (newMode == DRAW_PLATFORM) { 
        drawingPlatform = false;
    }
    mode = newMode;
}

void Editor::handleUserInput() {

    if (mode == DRAW_PLATFORM) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (!drawingPlatform) {
                drawingPlatform = true;
                drawPlatformStart = GetMousePosition();
            }
            else {
                drawPlatformEnd = GetMousePosition();
                drawingPlatform = false;
                finishedDrawingPlatform = true;
            }
        }
    }

    if (mode == MOVE_PLATFORM) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            searchingForSelectedPlatform = true;
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && (selectedPlatform != nullptr)) {
            Vector2 mouseDelta = GetMouseDelta();
            selectedPlatform->rectangle.x += mouseDelta.x;
            selectedPlatform->rectangle.y += mouseDelta.y;
        }
    }

}

Platform Editor::createDrawnPlatform() {
    Platform platform;
    platform.rectangle.x = (drawPlatformEnd.x < drawPlatformStart.x ? drawPlatformEnd.x : drawPlatformStart.x);
    platform.rectangle.y = (drawPlatformEnd.y < drawPlatformStart.y ? drawPlatformEnd.y : drawPlatformStart.y);
    platform.rectangle.width = abs(drawPlatformEnd.x - drawPlatformStart.x);
    platform.rectangle.height = abs(drawPlatformEnd.y - drawPlatformStart.y);
    return platform;
}   

void Editor::draw() {
    // std::cout << game.editor.mode << std::endl;
    if (GuiButton((Rectangle){ 30, 30, 30, 30 }, "#33#")) {
        if (mode != DRAW_PLATFORM) {
            setMode(DRAW_PLATFORM);
        }
    }

    if (GuiButton((Rectangle){ 70, 30, 30, 30 }, "#67#")) {
        if (mode != MOVE_PLATFORM) {
            setMode(MOVE_PLATFORM);
        }
    }

    if (mode == DRAW_PLATFORM && drawingPlatform) {
        Rectangle previewRectangle;
        Vector2 currentMousePosition = GetMousePosition(); 
        previewRectangle.x = (currentMousePosition.x < drawPlatformStart.x ? currentMousePosition.x : drawPlatformStart.x);
        previewRectangle.y = (currentMousePosition.y < drawPlatformStart.y ? currentMousePosition.y : drawPlatformStart.y);
        previewRectangle.width = abs(currentMousePosition.x - drawPlatformStart.x);
        previewRectangle.height = abs(currentMousePosition.y - drawPlatformStart.y);
        DrawRectangleRec(previewRectangle, drawPlatformColor);
    }

    if (mode == MOVE_PLATFORM && (selectedPlatform != nullptr)) {
        DrawRectangleLinesEx(selectedPlatform->rectangle, selectedLineThickness, SKYBLUE);
    }

}