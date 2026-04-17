#include <Arduino.h>
#include "encoder.h"
#include "main_display.h"
static DisplayState gDisplayState{};

static const char gKeyLabels[8][DISPLAY_LABEL_LEN + 1] = {
    "K1",
    "K2",
    "K3",
    "K4",
    "K5",
    "K6",
    "K7",
    "K8"
};

static EncoderBindings gEncoderBindings = {
    { KEY_RIGHT,  MOD_SHIFT },   // CW
    { KEY_LEFT,  MOD_SHIFT }    // CCW
};

void setup() {
    Serial.begin(115200);
    delay(1000);
    usbHidBegin();

    if (encoderBegin()) {
        Serial.println("Encoder init OK");
    } else {
        Serial.println("Encoder init FAILED");
    }
        if (!displayBegin()) {
        Serial.println("Display init failed");
        while (1) {
            delay(100);
        }
    }

    displaySetProfileName(gDisplayState, "PROF1");
    displaySetLastCommand(gDisplayState, "READY");
    displayFillFromKeyLabels(gDisplayState, gKeyLabels);

    displayRender(gDisplayState);
}

void loop() {
    encoderUpdate();

if (encoderHasSteps()) {
    int8_t steps = getPendingEncoderSteps();

    if (steps > 0) {
        for (int8_t i = 0; i < steps; i++) {
            usbHidSendKeypress(gEncoderBindings.clockwise.keyId,
                               gEncoderBindings.clockwise.modifiers);
        }
    } else if (steps < 0) {
        for (int8_t i = 0; i < -steps; i++) {
            usbHidSendKeypress(gEncoderBindings.counterClockwise.keyId,
                               gEncoderBindings.counterClockwise.modifiers);
        }
    }
}

        Serial.print("  angle=");
        Serial.print(getEncoderAngle());
        Serial.print("  diff=");
        Serial.print(getEncoderDiff());
        Serial.print("  count=");
        Serial.println(getEncoderCount());
    

    delay(2);
}

