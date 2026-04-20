// --- Transport layer. Designed for hanling keypress and encoder events
// PRimarily linking the keypress and encoder events into the menu and HID systems.
#include "action_layer.h"

#include <Arduino.h>
#include <string.h>

#include "encoder.h"
#include "interface.h"

namespace {

constexpr uint32_t SYSTEM_BUTTON_HOLD_MS = 500;

uint8_t clampProfileIndex(const KeymapConfig* config, uint8_t index) {
    if (config == nullptr || config->profileCount == 0) {
        return 0;
    }

    if (index >= config->profileCount) {
        return 0;
    }

    return index;
}

void applyActivePreset(ActionLayerState& state, uint8_t newPreset) {
    if (state.config == nullptr || state.deviceState == nullptr) {
        return;
    }

    if (state.config->profileCount == 0) {
        return;
    }

    newPreset = clampProfileIndex(state.config, newPreset);

    uint8_t current = clampProfileIndex(state.config, state.config->activeProfile);

    if (newPreset != current) {
        state.deviceState->previousPreset = current;
    }

    state.deviceState->activePreset = newPreset;
    state.config->activeProfile = newPreset;

    encoderSetStepCounts(
        state.config->profiles[newPreset].encoders[0].stepsPerDetent
    );

    Serial.print("[PRESET] active=");
    Serial.print(newPreset);
    Serial.print(" previous=");
    Serial.println(state.deviceState->previousPreset);
}

void togglePreviousPreset(ActionLayerState& state) {
    if (state.config == nullptr || state.deviceState == nullptr) {
        return;
    }

    if (state.config->profileCount < 2) {
        return;
    }

    uint8_t current = clampProfileIndex(state.config, state.config->activeProfile);
    uint8_t previous = clampProfileIndex(state.config, state.deviceState->previousPreset);

    if (previous == current) {
        previous = (current == 0) ? 1 : 0;
    }

    applyActivePreset(state, previous);
}

void enterPresetPicker(ActionLayerState& state) {
    if (state.config == nullptr) {
        return;
    }

    state.mode = MODE_PRESET_PICKER;
    state.highlightedPreset = clampProfileIndex(state.config, state.config->activeProfile);

    Serial.print("[PRESET] picker open, highlight=");
    Serial.println(state.highlightedPreset);
}

void exitToNormal(ActionLayerState& state) {
    state.mode = MODE_NORMAL;
    state.systemHoldHandled = false;
    state.systemButtonPressMs = 0;
    Serial.println("[PRESET] picker exit");
}

void updateSystemButtonModeTransitions(ActionLayerState& state) {
    if (wasSystemButtonPressed()) {
        state.systemButtonPressMs = millis();
        state.systemHoldHandled = false;
    }

    if (isSystemButtonDown() &&
        !state.systemHoldHandled &&
        state.mode == MODE_NORMAL) {

        uint32_t heldMs = millis() - state.systemButtonPressMs;
        if (heldMs >= SYSTEM_BUTTON_HOLD_MS) {
            state.systemHoldHandled = true;
            enterPresetPicker(state);
        }
    }

    if (wasSystemButtonReleased()) {
        if (state.mode == MODE_NORMAL) {
            if (!state.systemHoldHandled) {
                togglePreviousPreset(state);
            }
        } else if (state.mode == MODE_PRESET_PICKER) {
            applyActivePreset(state, state.highlightedPreset);
            exitToNormal(state);
        }

        state.systemHoldHandled = false;
        state.systemButtonPressMs = 0;
    }
}

void handleNormalButtons(ActionLayerState& state) {
    if (state.config == nullptr || state.config->profileCount == 0) {
        return;
    }

    uint8_t active = clampProfileIndex(state.config, state.config->activeProfile);

    for (uint8_t phys = 0; phys < NUM_BUTTONS; phys++) {
        PhysicalButtonId button = static_cast<PhysicalButtonId>(phys);

        if (!wasButtonPressed(button)) {
            continue;
        }

        LogicalKeySlot logical = physicalToLogical(button, state.orientation);

        const KeyAction& action = state.config->profiles[active].keys[logical];
        interfaceSendKeypress(action.KeyID, action.modifiers);
    }
}

void handleNormalEncoder(ActionLayerState& state) {
    if (!encoderHasSteps()) {
        return;
    }

    if (state.config == nullptr || state.config->profileCount == 0) {
        (void)getPendingEncoderSteps();
        return;
    }

    int8_t steps = getPendingEncoderSteps();
    uint8_t active = clampProfileIndex(state.config, state.config->activeProfile);

    const EncoderBinding& enc = state.config->profiles[active].encoders[0];
    bool invert = enc.invertDirection;

    while (steps > 0) {
        const KeyAction& action = invert ? enc.counterClockwise : enc.clockwise;
        interfaceSendKeypress(action.KeyID, action.modifiers);
        steps--;
    }

    while (steps < 0) {
        const KeyAction& action = invert ? enc.clockwise : enc.counterClockwise;
        interfaceSendKeypress(action.KeyID, action.modifiers);
        steps++;
    }
}

void handleNormalMode(ActionLayerState& state) {
    handleNormalButtons(state);
    handleNormalEncoder(state);
}

void handlePresetPickerMode(ActionLayerState& state) {
    if (state.config == nullptr || state.config->profileCount == 0) {
        return;
    }

    if (encoderHasSteps()) {
        int8_t steps = getPendingEncoderSteps();

        while (steps > 0) {
            if (state.highlightedPreset + 1 < state.config->profileCount) {
                state.highlightedPreset++;
            }
            steps--;
        }

        while (steps < 0) {
            if (state.highlightedPreset > 0) {
                state.highlightedPreset--;
            }
            steps++;
        }
    }

    // Optional temporary back key while testing
    for (uint8_t phys = 0; phys < NUM_BUTTONS; phys++) {
        PhysicalButtonId button = static_cast<PhysicalButtonId>(phys);

        if (!wasButtonPressed(button)) {
            continue;
        }

        LogicalKeySlot logical = physicalToLogical(button, state.orientation);

        if (logical == KEY_TOP_1) {
            exitToNormal(state);
            return;
        }
    }
}

const char* getProfileNameSafe(const KeymapConfig* config, uint8_t index) {
    if (config == nullptr || config->profileCount == 0) {
        return "------";
    }

    if (index >= config->profileCount) {
        return "------";
    }

    return config->profiles[index].name;
}

} // namespace

void actionLayerBegin(ActionLayerState& state,
                      KeymapConfig& config,
                      DeviceState& deviceState,
                      OrientationMode orientation) {
    state.mode = MODE_NORMAL;
    state.orientation = orientation;
    state.highlightedPreset = clampProfileIndex(&config, config.activeProfile);
    state.systemButtonPressMs = 0;
    state.systemHoldHandled = false;
    state.config = &config;
    state.deviceState = &deviceState;

    state.deviceState->activePreset = clampProfileIndex(&config, config.activeProfile);
    state.deviceState->previousPreset = clampProfileIndex(&config, deviceState.previousPreset);
}

void actionLayerSetOrientation(ActionLayerState& state, OrientationMode orientation) {
    state.orientation = orientation;
}

void actionLayerUpdate(ActionLayerState& state) {

    buttonsUpdate();
    systemButtonUpdate();
    debugPrintButtonPresses(state.orientation);
    encoderUpdate();

    updateSystemButtonModeTransitions(state);

    switch (state.mode) {
        case MODE_PRESET_PICKER:
            handlePresetPickerMode(state);
            break;

        case MODE_PRESET_EDIT:
            // later
            break;

        case MODE_NORMAL:
        default:
            handleNormalMode(state);
            break;
    }
}

bool actionLayerInMenu(const ActionLayerState& state) {
    return (state.mode != MODE_NORMAL);
}

const char* actionLayerGetMenuTitle(const ActionLayerState& state) {
    switch (state.mode) {
        case MODE_PRESET_PICKER:
            return "PICKER";

        case MODE_PRESET_EDIT:
            return "EDIT";

        case MODE_NORMAL:
        default:
            return "NORMAL";
    }
}

const char* actionLayerGetMenuItemLabel(const ActionLayerState& state, uint8_t index) {
    if (state.mode != MODE_PRESET_PICKER || state.config == nullptr) {
        return "------";
    }

    if (index >= 4) {
        return "------";
    }

    uint8_t item = state.highlightedPreset;

    if (index == 0 && item > 0) {
        return getProfileNameSafe(state.config, item - 1);
    }

    if (index == 1) {
        return getProfileNameSafe(state.config, item);
    }

    if (index == 2 && (item + 1) < state.config->profileCount) {
        return getProfileNameSafe(state.config, item + 1);
    }

    return "------";
}