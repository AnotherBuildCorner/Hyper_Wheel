#pragma once
#include <stdint.h>
#include "key_constants.h"

constexpr uint8_t NUM_KEYS = 8;
constexpr uint8_t LABEL_LEN = 6;
constexpr uint8_t MAX_PROFILES = 10;


struct KeyAction {
    uint16_t KeyID;                   // HID keycode, not ASCII
    uint8_t modifiers;                 // bitmask using ModifierFlags
    char label[LABEL_LEN + 1];         // null-terminated string
};

struct KeymapProfile {
    char name[LABEL_LEN + 1];          // optional profile name
    KeyAction keys[NUM_KEYS];
};

struct KeymapConfig {
    uint8_t version;
    uint8_t activeProfile;
    uint8_t profileCount;
    KeymapProfile profiles[MAX_PROFILES];
    uint32_t crc32;                    // optional integrity check
};
