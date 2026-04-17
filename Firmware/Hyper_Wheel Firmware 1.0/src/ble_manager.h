#pragma once

#include <Arduino.h>

namespace BleManager {

// Call once during setup
void begin(const char* deviceName);

// Call from loop if you later need periodic BLE-side housekeeping
void update();

// Connection state
bool isConnected();

// Send a raw text notification to the host, if connected
bool sendText(const char* msg);
bool sendText(const String& msg);

// Convenience publishers
bool publishKeyEvent(uint8_t keyId, bool pressed);
bool publishEncoderDelta(uint8_t encoderId, int32_t delta);
bool publishProfileChanged(uint8_t profileId);
bool publishCommandIssued(const char* commandName);

// Optional: last received command from RX characteristic
bool hasPendingCommand();
String getPendingCommand();
void clearPendingCommand();

}  // namespace BleManager