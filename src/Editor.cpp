#include "Editor.h"

void Editor::handleUserInput() {
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_Q)) {
        isActive = !isActive;
    }

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
    if (mode == DRAW_PLATFORM && drawingPlatform) {
        Vector2 currentMousePosition = GetMousePosition(); 
        Vector2 mousePositionDifference = Vector2Subtract(currentMousePosition, drawPlatformStart);
        if ((mousePositionDifference.x <= 0 && mousePositionDifference.y <= 0) || (mousePositionDifference.x > 0 && mousePositionDifference.y > 0)) {
            DrawRectangleV(drawPlatformStart, Vector2Subtract(currentMousePosition, drawPlatformStart), drawPlatformColor);
        }
        else if (mousePositionDifference.x < 0 && mousePositionDifference.y > 0) {
            DrawRectangle(currentMousePosition.x, 
                          currentMousePosition.y - mousePositionDifference.y, 
                          abs(mousePositionDifference.x),
                          abs(mousePositionDifference.y),
                          drawPlatformColor);
        }
        else if (mousePositionDifference.x > 0 && mousePositionDifference.y < 0) {
            DrawRectangle(currentMousePosition.x - mousePositionDifference.x, 
                          currentMousePosition.y, 
                          abs(mousePositionDifference.x),
                          abs(mousePositionDifference.y),
                          drawPlatformColor);
        }
    }
}