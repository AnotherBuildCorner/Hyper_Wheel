#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "buttons.h"
#include "keymap.h"

enum InputMode : uint8_t {
    MODE_NORMAL = 0,
    MODE_MENU
};

enum MenuPage : uint8_t {
    MENU_PAGE_ROOT = 0
};

enum MenuAction : uint8_t {
    MENU_ACTION_NONE = 0,
    MENU_ACTION_PROFILE,
    MENU_ACTION_PAIRING,
    MENU_ACTION_SETTINGS,
    MENU_ACTION_EXIT,
    MENU_ACTION_COUNT
};

struct ActionLayerState {
    InputMode mode;
    OrientationMode orientation;

    // Menu chord tracking
    bool menuChordArmed;
    uint32_t menuChordStartMs;

    // Menu state
    MenuPage menuPage;
    uint8_t menuIndex;

    // Current keymap/config pointer
    KeymapConfig* config;
};

void actionLayerBegin(ActionLayerState& state,
                      KeymapConfig& config,
                      OrientationMode orientation);

void actionLayerSetOrientation(ActionLayerState& state, OrientationMode orientation);

void actionLayerUpdate(ActionLayerState& state);

bool actionLayerInMenu(const ActionLayerState& state);
const char* actionLayerGetMenuTitle(const ActionLayerState& state);
const char* actionLayerGetMenuItemLabel(const ActionLayerState& state, uint8_t index);