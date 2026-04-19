#pragma once
#include <stdint.h>

constexpr uint8_t NUM_BUTTONS = 8;
constexpr uint16_t DEBOUNCE_MS = 20;

enum PhysicalButtonId : uint8_t {
    PHYS_BTN_1 = 0,
    PHYS_BTN_2,
    PHYS_BTN_3,
    PHYS_BTN_4,
    PHYS_BTN_5,
    PHYS_BTN_6,
    PHYS_BTN_7,
    PHYS_BTN_8
};

enum LogicalKeySlot : uint8_t {
    KEY_TOP_LEFT = 0,
    KEY_TOP_1,
    KEY_TOP_2,
    KEY_TOP_RIGHT,
    KEY_BOTTOM_LEFT,
    KEY_BOTTOM_1,
    KEY_BOTTOM_2,
    KEY_BOTTOM_RIGHT
};

enum OrientationMode : uint8_t {
    ORIENT_0_DEG = 0,
    ORIENT_180_DEG
};
// Maps the physical GPIO to the internal logic layer
LogicalKeySlot physicalToLogical(PhysicalButtonId button, OrientationMode orientation);
// returns the GPIO pin for a given physical button
uint8_t buttonToPin(PhysicalButtonId button);

// launches the button subsystem, call once during setup
void buttonsBegin();
// call periodically from loop to update button states and events
void buttonsUpdate();

// Query functions for button states and events. These will be cleared on the next buttonsUpdate() call.
bool wasButtonPressed(PhysicalButtonId id);
bool wasButtonReleased(PhysicalButtonId id);
bool isButtonDown(PhysicalButtonId id);
void debugPrintButtonPresses(OrientationMode orientation);