#pragma once
#include <stdint.h>
#include "key_constants.h"

constexpr uint8_t NUM_KEYS = 8;
constexpr uint8_t LABEL_LEN = 6;
constexpr uint8_t MAX_PROFILES = 10;
constexpr uint8_t MAX_ENCODERS = 2;

struct KeyAction {
    KeyId KeyID;
    uint8_t modifiers;
    char label[LABEL_LEN + 1];
};

struct EncoderBinding {
    KeyAction clockwise;
    KeyAction counterClockwise;
    uint16_t stepsPerDetent;
    uint8_t acceleration;
    bool invertDirection;
};

struct KeymapProfile {
    char name[LABEL_LEN + 1];
    KeyAction keys[NUM_KEYS];
    EncoderBinding encoders[MAX_ENCODERS];
};

struct KeymapConfig {
    uint8_t version;
    uint8_t activeProfile;
    uint8_t profileCount;
    KeymapProfile profiles[MAX_PROFILES];
    uint32_t crc32;
};


extern KeymapConfig gTestKeymapConfig;

extern const KeymapConfig gDefaultKeymapConfig;

void copyDefaultKeymapConfig(KeymapConfig& outConfig);