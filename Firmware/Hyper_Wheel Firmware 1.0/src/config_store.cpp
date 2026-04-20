#include "config_store.h"

#include <string.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>

#include "keymap.h"

using namespace Adafruit_LittleFS_Namespace;

namespace {

constexpr const char* CONFIG_FILENAME = "/hyperwheel.cfg";
constexpr const char* STATE_FILENAME  = "/hyperwheel.state";

constexpr uint32_t CONFIG_VERSION = 1;
constexpr uint32_t STATE_VERSION  = 1;

File gFile(InternalFS);

uint32_t crc32Update(uint32_t crc, const uint8_t* data, size_t len) {
    crc = ~crc;
    while (len--) {
        crc ^= *data++;
        for (uint8_t i = 0; i < 8; i++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320UL;
            } else {
                crc >>= 1;
            }
        }
    }
    return ~crc;
}

uint32_t computeConfigCrc32(const KeymapConfig& config) {
    KeymapConfig temp = config;
    temp.crc32 = 0;
    return crc32Update(0, reinterpret_cast<const uint8_t*>(&temp), sizeof(KeymapConfig));
}

uint32_t computeStateCrc32(const DeviceState& state) {
    DeviceState temp = state;
    temp.crc32 = 0;
    return crc32Update(0, reinterpret_cast<const uint8_t*>(&temp), sizeof(DeviceState));
}

} // namespace

bool configStoreBegin() {
    return InternalFS.begin();
}

// ------------------------------
// Defaults
// ------------------------------
void configStoreLoadDefaultConfig(KeymapConfig& outConfig) {
    copyDefaultKeymapConfig(outConfig);
}

void configStoreLoadDefaultState(DeviceState& outState) {
    copyDefaultDeviceState(outState);
}

// ------------------------------
// Validation
// ------------------------------
bool configStoreIsValidConfig(const KeymapConfig& config) {
    if (config.version != CONFIG_VERSION) {
        return false;
    }

    if (config.profileCount == 0 || config.profileCount > MAX_PROFILES) {
        return false;
    }

    if (config.activeProfile >= config.profileCount) {
        return false;
    }

    return (computeConfigCrc32(config) == config.crc32);
}

bool configStoreIsValidState(const DeviceState& state) {
    if (state.version != STATE_VERSION) {
        return false;
    }

    if (state.activePreset >= MAX_PROFILES) {
        return false;
    }

    if (state.previousPreset >= MAX_PROFILES) {
        return false;
    }

    return (computeStateCrc32(state) == state.crc32);
}

// ------------------------------
// Config blob
// ------------------------------
bool configStoreLoadConfig(KeymapConfig& outConfig) {
    memset(&outConfig, 0, sizeof(KeymapConfig));

    if (!gFile.open(CONFIG_FILENAME, FILE_O_READ)) {
        return false;
    }

    size_t readLen = gFile.read(reinterpret_cast<uint8_t*>(&outConfig), sizeof(KeymapConfig));
    gFile.close();

    if (readLen != sizeof(KeymapConfig)) {
        return false;
    }

    return configStoreIsValidConfig(outConfig);
}

bool configStoreSaveConfig(const KeymapConfig& config) {
    KeymapConfig temp = config;
    temp.version = CONFIG_VERSION;
    temp.crc32 = computeConfigCrc32(temp);

    if (!gFile.open(CONFIG_FILENAME, FILE_O_WRITE)) {
        return false;
    }

    size_t written = gFile.write(reinterpret_cast<const uint8_t*>(&temp), sizeof(KeymapConfig));
    gFile.close();

    return (written == sizeof(KeymapConfig));
}

// ------------------------------
// Device state blob
// ------------------------------
bool configStoreLoadState(DeviceState& outState) {
    memset(&outState, 0, sizeof(DeviceState));

    if (!gFile.open(STATE_FILENAME, FILE_O_READ)) {
        return false;
    }

    size_t readLen = gFile.read(reinterpret_cast<uint8_t*>(&outState), sizeof(DeviceState));
    gFile.close();

    if (readLen != sizeof(DeviceState)) {
        return false;
    }

    return configStoreIsValidState(outState);
}

bool configStoreSaveState(const DeviceState& state) {
    DeviceState temp = state;
    temp.version = STATE_VERSION;
    temp.crc32 = computeStateCrc32(temp);

    if (!gFile.open(STATE_FILENAME, FILE_O_WRITE)) {
        return false;
    }

    size_t written = gFile.write(reinterpret_cast<const uint8_t*>(&temp), sizeof(DeviceState));
    gFile.close();

    return (written == sizeof(DeviceState));
}