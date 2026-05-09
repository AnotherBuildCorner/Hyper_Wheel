#pragma once

#include <Arduino.h>
#include <stdint.h>
#include "usb_hid.h"

// -----------------------------------------------------------------------------
// Encoder selection
// -----------------------------------------------------------------------------

enum EncoderId : uint8_t {
    ENCODER_A = 0,
    ENCODER_B = 1
};

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

// Generic per-encoder API
int8_t getPendingEncoderSteps(EncoderId id);
bool encoderHasSteps(EncoderId id);

long getEncoderCount(EncoderId id);
uint16_t getEncoderAngle(EncoderId id);
int16_t getEncoderDiff(EncoderId id);

void encoderSetStepCounts(EncoderId id, uint16_t stepCounts);

// Optional compatibility wrappers / convenience API
inline int8_t getPendingEncoderASteps() {
    return getPendingEncoderSteps(ENCODER_A);
}

inline int8_t getPendingEncoderBSteps() {
    return getPendingEncoderSteps(ENCODER_B);
}

inline bool encoderAHasSteps() {
    return encoderHasSteps(ENCODER_A);
}

inline bool encoderBHasSteps() {
    return encoderHasSteps(ENCODER_B);
}

inline void encoderASetStepCounts(uint16_t stepCounts) {
    encoderSetStepCounts(ENCODER_A, stepCounts);
}

inline void encoderBSetStepCounts(uint16_t stepCounts) {
    encoderSetStepCounts(ENCODER_B, stepCounts);
}