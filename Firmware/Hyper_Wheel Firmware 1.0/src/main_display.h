#pragma once

#include <stdint.h>

struct ActionLayerState;

constexpr uint8_t DISPLAY_LABEL_LEN = 6;

struct DisplayState {
    char profileName[DISPLAY_LABEL_LEN + 1];
    char lastCommand[DISPLAY_LABEL_LEN + 1];

    char screenA[4][DISPLAY_LABEL_LEN + 1]; // 5,6,1,2
    char screenB[4][DISPLAY_LABEL_LEN + 1]; // 7,8,3,4
};

bool displayBegin();

void displayRenderNormal(const DisplayState& state);
void displayRenderPresetPicker(const ActionLayerState& actionState);

void displaySetProfileName(DisplayState& state, const char* text);
void displaySetLastCommand(DisplayState& state, const char* text);
void displayFillFromKeyLabels(DisplayState& state,
                              const char keyLabels[8][DISPLAY_LABEL_LEN + 1]);