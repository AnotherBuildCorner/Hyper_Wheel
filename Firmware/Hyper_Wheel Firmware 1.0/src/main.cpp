#include <Arduino.h>

#include "usb_hid.h"
#include "buttons.h"
#include "encoder.h"
#include "main_display.h"
#include "action_layer.h"
#include "keymap.h"
#include "key_constants.h"

static DisplayState gDisplayState{};
static ActionLayerState gActionState{};

static KeymapConfig gConfig = {
    .version = 1,
    .activeProfile = 0,
    .profileCount = 1,
    .profiles = {{
        "PROF1",
{
    { KEY_A, 0, "K1 A" },
    { KEY_B, 0, "K2 B" },
    { KEY_C, 0, "K3 C" },
    { KEY_D, 0, "K4 D" },
    { KEY_E, 0, "K5 E" },
    { KEY_F, 0, "K6 F" },
    { KEY_G, 0, "K7 G" },
    { KEY_H, 0, "K8 H" }
}
    }},
    .crc32 = 0
};

static void fillDisplayFromActiveProfile(DisplayState& state, const KeymapConfig& config) {
    char labels[8][DISPLAY_LABEL_LEN + 1] = {};

    const KeymapProfile& profile = config.profiles[config.activeProfile];

    for (uint8_t i = 0; i < 8; i++) {
        strncpy(labels[i], profile.keys[i].label, DISPLAY_LABEL_LEN);
        labels[i][DISPLAY_LABEL_LEN] = '\0';
    }

    displaySetProfileName(state, profile.name);
    displaySetLastCommand(state, "READY");
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

    usbHidBegin();
    buttonsBegin();
    encoderBegin();

    if (!displayBegin()) {
        Serial.println("Display init failed");
        while (1) {
            delay(100);
        }
    }

    actionLayerBegin(gActionState, gConfig, ORIENT_0_DEG);

    fillDisplayFromActiveProfile(gDisplayState, gConfig);
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
        fillDisplayFromActiveProfile(gDisplayState, gConfig);
    }
if(draw == false){
    displayRender(gDisplayState);
draw = true;}

    

    delay(2);
}