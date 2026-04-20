#pragma once

#include <Arduino.h>
#include <stdint.h>
#include "usb_hid.h"
// -----------------------------------------------------------------------------
// Action model
// -----------------------------------------------------------------------------
enum ActionType : uint8_t {
    ACTION_NONE = 0,
    ACTION_KEYBOARD,
    ACTION_CONSUMER,
    ACTION_MOUSE_WHEEL
};

struct Action {
    ActionType type;
    uint16_t code;
    int8_t value;
    uint8_t modifiers;
};


struct EncoderKeybind {
    KeyId keyId;
    uint8_t modifiers;
};

struct EncoderBindings {
    EncoderKeybind clockwise;
    EncoderKeybind counterClockwise;
};

// -----------------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------------
bool encoderBegin();
void encoderUpdate();

int8_t getPendingEncoderSteps();
bool encoderHasSteps();

Action getEncoderAction();

long getEncoderCount();
uint16_t getEncoderAngle();
int16_t getEncoderDiff();

void encoderSetStepCounts(uint16_t stepCounts);