#include <Arduino.h>
#include <string.h>

#include "buttons.h"
#include "encoder.h"
#include "main_display.h"
#include "action_layer.h"
#include "keymap.h"
#include "interface.h"
#include "config_store.h"

static DisplayState gDisplayState{};
static ActionLayerState gActionState{};
static DeviceState gDeviceState{};

static void fillDisplayFromActiveProfile(DisplayState& state, const KeymapConfig& config) {
    char labels[8][DISPLAY_LABEL_LEN + 1] = {};

    const KeymapProfile& profile = config.profiles[config.activeProfile];

    for (uint8_t i = 0; i < 8; i++) {
        strncpy(labels[i], profile.keys[i].label, DISPLAY_LABEL_LEN);
        labels[i][DISPLAY_LABEL_LEN] = '\0';
    }

    displaySetProfileName(state, profile.name);
    displaySetLastCommand(
        state,
        config.profiles[config.activeProfile].encoders[0].clockwise.label
    );
    displayFillFromKeyLabels(state, labels);
}

static void fillDisplayForPresetPicker(DisplayState& state) {
    char labels[8][DISPLAY_LABEL_LEN + 1] = {
        "PREV",
        "CURR",
        "NEXT",
        "------",
        "------",
        "------",
        "------",
        "BACK"
    };

    displaySetProfileName(state, "PICKER");
    displaySetLastCommand(state, actionLayerGetMenuTitle(gActionState));

    strncpy(labels[0], actionLayerGetMenuItemLabel(gActionState, 0), DISPLAY_LABEL_LEN);
    labels[0][DISPLAY_LABEL_LEN] = '\0';

    strncpy(labels[1], actionLayerGetMenuItemLabel(gActionState, 1), DISPLAY_LABEL_LEN);
    labels[1][DISPLAY_LABEL_LEN] = '\0';

    strncpy(labels[2], actionLayerGetMenuItemLabel(gActionState, 2), DISPLAY_LABEL_LEN);
    labels[2][DISPLAY_LABEL_LEN] = '\0';

    displayFillFromKeyLabels(state, labels);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    interfaceBegin();
    buttonsBegin();
    systemButtonBegin();
    encoderBegin();

    if (!displayBegin()) {
        Serial.println("Display init failed");
        while (1) {
            delay(100);
        }
    }

    //copyDefaultKeymapConfig(gDefaultKeymapConfig);
    configStoreLoadDefaultState(gDeviceState);

    gDeviceState.activePreset = gDefaultKeymapConfig.activeProfile;
    if (gDefaultKeymapConfig.profileCount > 1) {
        gDeviceState.previousPreset =
            (gDefaultKeymapConfig.activeProfile == 0) ? 1 : 0;
    } else {
        gDeviceState.previousPreset = gDefaultKeymapConfig.activeProfile;
    }

    actionLayerBegin(
        gActionState,
        gDefaultKeymapConfig,
        gDeviceState,
        ORIENT_0_DEG
    );

    encoderSetStepCounts(
        gDefaultKeymapConfig.profiles[gDefaultKeymapConfig.activeProfile]
            .encoders[0]
            .stepsPerDetent
    );

    fillDisplayFromActiveProfile(gDisplayState, gDefaultKeymapConfig);
    displayRender(gDisplayState);
}

void loop() {
    actionLayerUpdate(gActionState);

    if (actionLayerInMenu(gActionState)) {
        fillDisplayForPresetPicker(gDisplayState);
    } else {
        fillDisplayFromActiveProfile(gDisplayState, gDefaultKeymapConfig);
    }

    displayRender(gDisplayState);
    delay(2);
}