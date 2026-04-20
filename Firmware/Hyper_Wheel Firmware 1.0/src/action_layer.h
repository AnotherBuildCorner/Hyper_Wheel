#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "buttons.h"
#include "keymap.h"
#include "config_store.h"

enum InputMode : uint8_t {
    MODE_NORMAL = 0,
    MODE_PRESET_PICKER,
    MODE_PRESET_EDIT
};

struct ActionLayerState {
    InputMode mode;
    OrientationMode orientation;

    uint8_t highlightedPreset;

    uint32_t systemButtonPressMs;
    bool systemHoldHandled;
    bool suppressNextPickerRelease;

    KeymapConfig* config;
    DeviceState* deviceState;
};

void actionLayerBegin(ActionLayerState& state,
                      KeymapConfig& config,
                      DeviceState& deviceState,
                      OrientationMode orientation);

void actionLayerSetOrientation(ActionLayerState& state, OrientationMode orientation);
bool actionLayerUpdate(ActionLayerState& state);

bool actionLayerInMenu(const ActionLayerState& state);
const char* actionLayerGetMenuTitle(const ActionLayerState& state);
const char* actionLayerGetMenuItemLabel(const ActionLayerState& state, uint8_t index);