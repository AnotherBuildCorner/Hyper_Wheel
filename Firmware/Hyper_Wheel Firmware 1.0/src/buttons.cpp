#include "buttons.h"
#include <Arduino.h>

// --- Pinout for XIAO NRF52840 ---

constexpr uint8_t PIN_BTN_1 = 7;   // D7,  P1.12
constexpr uint8_t PIN_BTN_2 = 8;   // D8,  P1.13
constexpr uint8_t PIN_BTN_3 = 9;   // D9,  P1.14
constexpr uint8_t PIN_BTN_4 = 10;  // D10, P1.15
constexpr uint8_t PIN_BTN_5 = 6;   // D6,  P1.11
constexpr uint8_t PIN_BTN_6 = 3;   // D3,  P0.29
constexpr uint8_t PIN_BTN_7 = 2;   // D2,  P0.28
constexpr uint8_t PIN_BTN_8 = 1;   // D1,  P0.03

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

static constexpr LogicalKeySlot kOrientationMap[2][NUM_BUTTONS] = {
    {
        KEY_TOP_LEFT,
        KEY_TOP_1,
        KEY_TOP_2,
        KEY_TOP_RIGHT,
        KEY_BOTTOM_LEFT,
        KEY_BOTTOM_1,
        KEY_BOTTOM_2,
        KEY_BOTTOM_RIGHT
    },
    {
        KEY_BOTTOM_RIGHT,
        KEY_BOTTOM_2,
        KEY_BOTTOM_1,
        KEY_BOTTOM_LEFT,
        KEY_TOP_RIGHT,
        KEY_TOP_2,
        KEY_TOP_1,
        KEY_TOP_LEFT
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

bool wasButtonPressed(PhysicalButtonId id) {
    return gButtons[id].pressedEvent;
}

bool wasButtonReleased(PhysicalButtonId id) {
    return gButtons[id].releasedEvent;
}

bool isButtonDown(PhysicalButtonId id) {
    return gButtons[id].stableState;
}