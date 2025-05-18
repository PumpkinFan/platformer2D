#include "Editor.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


void EditorButton::draw() {
    if (GuiButton(bounds, text)) {
        onClick();
    }
}

#define DRAW_PLATFORM_BUTTON_ICON "#33#"
#define SELECT_PLATFORM_BUTTON_ICON "#67#"
#define FILE_SAVE_BUTTON_ICON "#02#"
#define FILE_LOAD_BUTTON_ICON "#01#"

std::vector<EditorButton> Editor::generateButtons() {
    std::vector<EditorButton> buttons = {};
    
    EditorButton drawPlatformButton = {
        {buttonsStart.x, buttonsStart.y, buttonSize, buttonSize},
        DRAW_PLATFORM_BUTTON_ICON,
        [this]() { 
            if (mode != DRAW_PLATFORM) {
                setMode(DRAW_PLATFORM);
            } 
        },

    };
    buttons.push_back(drawPlatformButton);

    EditorButton selectPlatformButton = {
        {buttonsStart.x + buttonSize + buttonPadding, buttonsStart.y, buttonSize, buttonSize},
        SELECT_PLATFORM_BUTTON_ICON,
        [this]() {
            if (mode != SELECT_PLATFORM) {
                setMode(SELECT_PLATFORM);
            }
        }
    };
    buttons.push_back(selectPlatformButton);

    EditorButton saveGameStateButton = {
        {buttonsStart.x + 2 * (buttonSize + buttonPadding), buttonsStart.y, buttonSize, buttonSize},
        FILE_SAVE_BUTTON_ICON,
        [this]() {
            return;
        }
    };
    buttons.push_back(saveGameStateButton);

    EditorButton loadGameStateButton = {
        {buttonsStart.x + 3 * (buttonSize + buttonPadding), buttonsStart.y, buttonSize, buttonSize},
        FILE_LOAD_BUTTON_ICON,
        [this]() {
            return;
        }
    };
    buttons.push_back(loadGameStateButton);

    return buttons;
}

void Editor::checkActiveToggle() {
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_Q)) {
        isActive = !isActive;
    }
}

void Editor::setMode(EditorMode newMode) {
    if (newMode == DRAW_PLATFORM) { 
        drawingPlatform = false;
    }
    if (newMode != SELECT_PLATFORM) {
        // forget the selected platform if we switch off SELECT_PLATFORM
        selectedPlatform = nullptr;
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

    if (mode == SELECT_PLATFORM) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            searchingForSelectedPlatform = true;
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && (selectedPlatform != nullptr)) {
            Vector2 mouseDelta = GetMouseDelta();
            selectedPlatform->rectangle.x += mouseDelta.x;
            selectedPlatform->rectangle.y += mouseDelta.y;
        }
        if (IsKeyPressed(KEY_GRAVE)) {  // Detect '~' key on U.S. layout
            selectedPlatform = nullptr;
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
    //----------------------------------------------------------------------------------
    // Draw raygui components
    //----------------------------------------------------------------------------------
    for (EditorButton button : buttons) {
        button.draw();
    }
    //----------------------------------------------------------------------------------
    // Draw other GUI elements
    //----------------------------------------------------------------------------------
    if (mode == DRAW_PLATFORM && drawingPlatform) {
        Rectangle previewRectangle;
        Vector2 currentMousePosition = GetMousePosition(); 
        previewRectangle.x = (currentMousePosition.x < drawPlatformStart.x ? currentMousePosition.x : drawPlatformStart.x);
        previewRectangle.y = (currentMousePosition.y < drawPlatformStart.y ? currentMousePosition.y : drawPlatformStart.y);
        previewRectangle.width = abs(currentMousePosition.x - drawPlatformStart.x);
        previewRectangle.height = abs(currentMousePosition.y - drawPlatformStart.y);
        DrawRectangleRec(previewRectangle, drawPlatformColor);
    }

    if (mode == SELECT_PLATFORM && (selectedPlatform != nullptr)) {
        DrawRectangleLinesEx(selectedPlatform->rectangle, selectedLineThickness, SKYBLUE);
    }

}