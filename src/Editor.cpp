#include "Editor.h"
#include <iostream>
#include <cstdio>
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
            setMode(ENTER_SAVE_PATH);
        }
    };
    buttons.push_back(saveGameStateButton);

    EditorButton loadGameStateButton = {
        {buttonsStart.x + 3 * (buttonSize + buttonPadding), buttonsStart.y, buttonSize, buttonSize},
        FILE_LOAD_BUTTON_ICON,
        [this]() {
            setMode(ENTER_LOAD_PATH);
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
        selectedObject = nullptr;
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
            searchingForSelectedObject = true;
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && (selectedObject != nullptr)) {
            Vector2 mouseDelta = GetMouseDelta();
            Rectangle rect = selectedObject->getRect();
            rect.x += mouseDelta.x;
            rect.y += mouseDelta.y;
            selectedObject->setRect(rect);
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && (selectedObject != nullptr)) {
            if (CheckCollisionPointRec(GetMousePosition(), selectedObject->getRect())) {
                contextMenuPosition = GetMousePosition();
                setMode(CONTEXT_MENU);
            }
        }

        if (IsKeyPressed(KEY_GRAVE)) {  // Detect '~' key on U.S. layout
            selectedObject = nullptr;
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

std::filesystem::path Editor::readSavePath() {
    savingGameState = false;
    return saveTargetPath;
}

std::filesystem::path Editor::readLoadPath() {
    loadingGameState = false;
    return loadTargetPath;
}

void Editor::draw() {
    //----------------------------------------------------------------------------------
    // Draw raygui components
    //----------------------------------------------------------------------------------
    for (EditorButton button : buttons) {
        button.draw();

    }
    bool textInputBoxSecretViewActive = false;
    if (mode == ENTER_SAVE_PATH) {
        int saveBoxInput = GuiTextInputBox(Rectangle { 100, 100, 500, 150 }, 
                                           "Save to File", 
                                           "Enter file path:",
                                           "Save; Cancel",
                                           tempSavePath,
                                           128,
                                           &textInputBoxSecretViewActive);
        if (saveBoxInput == 1) {
            saveTargetPath = std::filesystem::path(tempSavePath);
            setMode(DO_NOTHING);
            savingGameState = true;
            // tempSavePath[0] = '\0'; // could be used to clear the temporary path
        }
        else if (saveBoxInput == 2) {
            setMode(DO_NOTHING);
        }
    }
    if (mode == ENTER_LOAD_PATH) {
        int loadBoxInput = GuiTextInputBox(Rectangle { 100, 100, 500, 150 }, 
                                           "Load from File", 
                                           "Enter file path:",
                                           "Load; Cancel",
                                           tempLoadPath,
                                           128,
                                           &textInputBoxSecretViewActive);
        if (loadBoxInput == 1) {
            loadTargetPath = std::filesystem::path(tempLoadPath);
            setMode(DO_NOTHING);
            loadingGameState = true;
        }
        else if (loadBoxInput == 2) {
            setMode(DO_NOTHING);
        }
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

    if (mode == SELECT_PLATFORM && (selectedObject != nullptr)) {
        DrawRectangleLinesEx(selectedObject->getRect(), selectedLineThickness, LIME);
    }

    if (mode == CONTEXT_MENU) {
        int result = GuiListViewEx(Rectangle{ contextMenuPosition.x, contextMenuPosition.y, 100, 50 },
                                    contextMenuItems,
                                    2,
                                    &contextMenuFocus,
                                    &contextMenuScrollIndex,
                                    &contextMenuItemSelected);

        if (result != -1) {
            contextMenuItemSelected = result;
            if (contextMenuItemSelected == 0) { // Set Exact Value
                setMode(SET_EXACT_VALUES);
                Rectangle rect = selectedObject->getRect();
                snprintf(exactX, 10, "%f", rect.x);
                snprintf(exactY, 10, "%f", rect.y);
                snprintf(exactWidth, 10, "%f", rect.width);
                snprintf(exactHeight, 10, "%f", rect.height);
            }
            else if (contextMenuItemSelected == 1) { // Change Color
                // TODO: Implement change color functionality
            }
            contextMenuItemSelected = -1;
        }
    }

}