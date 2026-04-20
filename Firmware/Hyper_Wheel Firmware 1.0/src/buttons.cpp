// Primary control for individual buttons and primary map.

#include "buttons.h"
#include <Arduino.h>

// --- Pinout for XIAO NRF52840 ---



// --- physical pin constructor
static constexpr uint8_t kButtonPins[NUM_BUTTONS] = {
    PIN_BTN_1,
    PIN_BTN_2,
    PIN_BTN_3,
    PIN_BTN_4,
    PIN_BTN_5,
    PIN_BTN_6,
    PIN_BTN_7,
    PIN_BTN_8
};

// --- logical mapping for orientations. Logical keys are the internal representation of the buttons.
static constexpr LogicalKeySlot kOrientationMap[2][NUM_BUTTONS] = {
    {
        KEY_BOTTOM_LEFT,
        KEY_BOTTOM_1,
        KEY_BOTTOM_2,
        KEY_BOTTOM_RIGHT,
        KEY_TOP_LEFT,
        KEY_TOP_1,
        KEY_TOP_2,
        KEY_TOP_RIGHT

    },
    {
        KEY_TOP_RIGHT,
        KEY_TOP_2,
        KEY_TOP_1,
        KEY_TOP_LEFT,
        KEY_BOTTOM_RIGHT,
        KEY_BOTTOM_2,
        KEY_BOTTOM_1,
        KEY_BOTTOM_LEFT
    }
};

struct ButtonState {
    bool stableState;
    bool lastRawState;
    bool pressedEvent;
    bool releasedEvent;
    uint32_t lastChangeTime;
};

static ButtonState gButtons[NUM_BUTTONS];

LogicalKeySlot physicalToLogical(PhysicalButtonId button, OrientationMode orientation) {
    return kOrientationMap[orientation][button];
}

uint8_t buttonToPin(PhysicalButtonId button) {
    return kButtonPins[button];
}
// -- launch buttons 
void buttonsBegin() {
    for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
        pinMode(kButtonPins[i], INPUT_PULLUP);

        bool rawPressed = (digitalRead(kButtonPins[i]) == LOW);

        gButtons[i].stableState = rawPressed;
        gButtons[i].lastRawState = rawPressed;
        gButtons[i].pressedEvent = false;
        gButtons[i].releasedEvent = false;
        gButtons[i].lastChangeTime = millis();
    }
}
// Updates status of each button, call periodically from loop.

void buttonsUpdate() {
    uint32_t now = millis();

    for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
        gButtons[i].pressedEvent = false;
        gButtons[i].releasedEvent = false;

        bool rawPressed = (digitalRead(kButtonPins[i]) == LOW);

        if (rawPressed != gButtons[i].lastRawState) {
            gButtons[i].lastRawState = rawPressed;
            gButtons[i].lastChangeTime = now;
        }

        if ((now - gButtons[i].lastChangeTime) >= DEBOUNCE_MS) {
            if (gButtons[i].stableState != gButtons[i].lastRawState) {
                gButtons[i].stableState = gButtons[i].lastRawState;

                if (gButtons[i].stableState) {
                    gButtons[i].pressedEvent = true;
                } else {
                    gButtons[i].releasedEvent = true;
                }
            }
        }
    }
}


// Return button status types.
bool wasButtonPressed(PhysicalButtonId id) {
    return gButtons[id].pressedEvent;
}

bool wasButtonReleased(PhysicalButtonId id) {
    return gButtons[id].releasedEvent;
}

bool isButtonDown(PhysicalButtonId id) {
    return gButtons[id].stableState;
}

void debugPrintButtonPresses(OrientationMode orientation) {
    for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
        PhysicalButtonId phys = static_cast<PhysicalButtonId>(i);

        if (wasButtonPressed(phys)) {
            LogicalKeySlot logical = physicalToLogical(phys, orientation);

            Serial.print("[BTN] pressed phys=");
            Serial.print(i);
            Serial.print(" logical=");
            Serial.println(static_cast<uint8_t>(logical));
        }
    }
}

//--------------- System Button ----------------
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