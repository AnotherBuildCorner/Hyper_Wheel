#include <Arduino.h>

#include "buttons.h"
#include "encoder.h"
#include "main_display.h"
#include "action_layer.h"
#include "keymap.h"
#include "interface.h"

static DisplayState gDisplayState{};
static ActionLayerState gActionState{};

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

static void fillDisplayForMenu(DisplayState& state) {
    char labels[8][DISPLAY_LABEL_LEN + 1] = {
        "SEL",
        "BACK",
        "------",
        "------",
        "------",
        "------",
        "------",
        "------"
    };

    displaySetProfileName(state, "MENU");
    displaySetLastCommand(state, actionLayerGetMenuTitle(gActionState));
    displayFillFromKeyLabels(state, labels);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    interfaceBegin();
    buttonsBegin();
    encoderBegin();

    if (!displayBegin()) {
        Serial.println("Display init failed");
        while (1) {
            delay(100);
        }
    }

    actionLayerBegin(gActionState, gTestKeymapConfig, ORIENT_0_DEG);

    encoderSetStepCounts(
        gTestKeymapConfig.profiles[gTestKeymapConfig.activeProfile]
            .encoders[0]
            .stepsPerDetent
    );

    fillDisplayFromActiveProfile(gDisplayState, gTestKeymapConfig);
    displayRender(gDisplayState);
}

void loop() {
    static bool draw = false;
    actionLayerUpdate(gActionState);

    if (actionLayerInMenu(gActionState)) {
        fillDisplayForMenu(gDisplayState);
        displayRender(gDisplayState);
        draw = false;
    } else {
        fillDisplayFromActiveProfile(gDisplayState, gTestKeymapConfig);
    }

    if (!draw) {
        displayRender(gDisplayState);
        draw = true;
    }

    delay(2);
}