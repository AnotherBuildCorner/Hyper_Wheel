#include "system_button.h"
#include <Arduino.h>
#include "pins_and_constants.h"
// Onboard non-key system button
// This button is intentionally kept outside the 8-key logical matrix.

struct SystemButtonState {
    bool stableState;
    bool lastRawState;
    bool pressedEvent;
    bool releasedEvent;
    uint32_t lastChangeTime;
};

static SystemButtonState gSystemButton{};

void systemButtonBegin() {
    pinMode(PUSH_BTN_PIN, INPUT_PULLUP);

    bool rawPressed = (digitalRead(PUSH_BTN_PIN) == LOW);

    gSystemButton.stableState = rawPressed;
    gSystemButton.lastRawState = rawPressed;
    gSystemButton.pressedEvent = false;
    gSystemButton.releasedEvent = false;
    gSystemButton.lastChangeTime = millis();
}

void systemButtonUpdate() {
    uint32_t now = millis();

    gSystemButton.pressedEvent = false;
    gSystemButton.releasedEvent = false;

    bool rawPressed = (digitalRead(PUSH_BTN_PIN) == LOW);

    if (rawPressed != gSystemButton.lastRawState) {
        gSystemButton.lastRawState = rawPressed;
        gSystemButton.lastChangeTime = now;
    }

    if ((now - gSystemButton.lastChangeTime) >= SYSTEM_BUTTON_DEBOUNCE_MS) {
        if (gSystemButton.stableState != gSystemButton.lastRawState) {
            gSystemButton.stableState = gSystemButton.lastRawState;

            if (gSystemButton.stableState) {
                gSystemButton.pressedEvent = true;
            } else {
                gSystemButton.releasedEvent = true;
            }
        }
    }
}

bool wasSystemButtonPressed() {
    return gSystemButton.pressedEvent;
}

bool wasSystemButtonReleased() {
    return gSystemButton.releasedEvent;
}

bool isSystemButtonDown() {
    return gSystemButton.stableState;
}