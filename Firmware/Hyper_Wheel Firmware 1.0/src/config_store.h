#pragma once

#include <stdbool.h>
#include "keymap.h"


struct DeviceState {
    uint8_t version;
    uint8_t activePreset;
    uint8_t previousPreset;
    uint8_t lastHost;
    uint32_t crc32;
};

bool configStoreBegin();

// Full preset/config blob
bool configStoreLoadConfig(KeymapConfig& outConfig);
bool configStoreSaveConfig(const KeymapConfig& config);
void configStoreLoadDefaultConfig(KeymapConfig& outConfig);
bool configStoreIsValidConfig(const KeymapConfig& config);

// Small persistent device state
bool configStoreLoadState(DeviceState& outState);
bool configStoreSaveState(const DeviceState& state);
void configStoreLoadDefaultState(DeviceState& outState);
bool configStoreIsValidState(const DeviceState& state);