#include "ble_manager.h"

#include <bluefruit.h>

// =========================
// UUIDs
// =========================
// Use your own later. These are placeholder 128-bit UUIDs.
//
// Service UUID:
//   12345678-1234-5678-1234-56789abcdef0
// TX Char UUID:
//   12345678-1234-5678-1234-56789abcdef1
// RX Char UUID:
//   12345678-1234-5678-1234-56789abcdef2

namespace {

BLEService hwService = BLEService(0x181C);  // Placeholder. Replace with 128-bit UUID later if desired.
BLECharacteristic txChar = BLECharacteristic();
BLECharacteristic rxChar = BLECharacteristic();

volatile bool g_connected = false;
String g_pendingCommand;
bool g_hasPendingCommand = false;

constexpr size_t TX_MAX_LEN = 120;
constexpr size_t RX_MAX_LEN = 120;

// -------------------------
// Forward declarations
// -------------------------
void connect_callback(uint16_t conn_handle);
void disconnect_callback(uint16_t conn_handle, uint8_t reason);
void rx_write_callback(uint16_t conn_handle, BLECharacteristic* chr, uint8_t* data, uint16_t len);
void startAdvertising(const char* deviceName);

}  // namespace

namespace BleManager {

void begin(const char* deviceName) {
  Bluefruit.begin();
  Bluefruit.setTxPower(4);               // Conservative starting point
  Bluefruit.setName(deviceName);

  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  // Optional but useful during bring-up
  // Bluefruit.autoConnLed(true);

  // -------------------------
  // Service setup
  // -------------------------
  hwService = BLEService("12345678-1234-5678-1234-56789abcdef0");
  hwService.begin();

  // -------------------------
  // TX characteristic
  // Board -> host notifications
  // -------------------------
  txChar = BLECharacteristic("12345678-1234-5678-1234-56789abcdef1");
  txChar.setProperties(CHR_PROPS_NOTIFY);
  txChar.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  txChar.setFixedLen(TX_MAX_LEN);
  txChar.begin();

  // -------------------------
  // RX characteristic
  // Host -> board writes
  // -------------------------
  rxChar = BLECharacteristic("12345678-1234-5678-1234-56789abcdef2");
  rxChar.setProperties(CHR_PROPS_WRITE | CHR_PROPS_WRITE_WO_RESP);
  rxChar.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  rxChar.setFixedLen(RX_MAX_LEN);
  rxChar.setWriteCallback(rx_write_callback);
  rxChar.begin();

  startAdvertising(deviceName);
}

void update() {
  // Placeholder for future BLE-side periodic work.
  // Useful later if you want heartbeat/status packets.
}

bool isConnected() {
  return g_connected;
}

bool sendText(const char* msg) {
  if (!g_connected || msg == nullptr) {
    return false;
  }

  char buffer[TX_MAX_LEN] = {0};
  strncpy(buffer, msg, TX_MAX_LEN - 1);

  return txChar.notify((uint8_t*)buffer, TX_MAX_LEN);
}

bool sendText(const String& msg) {
  return sendText(msg.c_str());
}

bool publishKeyEvent(uint8_t keyId, bool pressed) {
  String msg = "KEY,";
  msg += keyId;
  msg += ",";
  msg += (pressed ? "DOWN" : "UP");
  return sendText(msg);
}

bool publishEncoderDelta(uint8_t encoderId, int32_t delta) {
  String msg = "ENC,";
  msg += encoderId;
  msg += ",";
  msg += delta;
  return sendText(msg);
}

bool publishProfileChanged(uint8_t profileId) {
  String msg = "PROFILE,";
  msg += profileId;
  return sendText(msg);
}

bool publishCommandIssued(const char* commandName) {
  String msg = "CMD,";
  msg += commandName ? commandName : "UNKNOWN";
  return sendText(msg);
}

bool hasPendingCommand() {
  return g_hasPendingCommand;
}

String getPendingCommand() {
  return g_pendingCommand;
}

void clearPendingCommand() {
  g_pendingCommand = "";
  g_hasPendingCommand = false;
}

}  // namespace BleManager

namespace {

void connect_callback(uint16_t conn_handle) {
  (void)conn_handle;
  g_connected = true;
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason) {
  (void)conn_handle;
  (void)reason;
  g_connected = false;

  // Restart advertising so reconnect is easy during bring-up
  Bluefruit.Advertising.start(0);
}

void rx_write_callback(uint16_t conn_handle, BLECharacteristic* chr, uint8_t* data, uint16_t len) {
  (void)conn_handle;
  (void)chr;

  if (data == nullptr || len == 0) {
    return;
  }

  if (len >= RX_MAX_LEN) {
    len = RX_MAX_LEN - 1;
  }

  char buffer[RX_MAX_LEN] = {0};
  memcpy(buffer, data, len);
  buffer[len] = '\0';

  g_pendingCommand = String(buffer);
  g_hasPendingCommand = true;
}

void startAdvertising(const char* deviceName) {
  Bluefruit.Advertising.stop();
  //Bluefruit.ScanResponse.stop();

  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(hwService);
  Bluefruit.ScanResponse.addName();

  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);   // fast, slow
  Bluefruit.Advertising.setFastTimeout(30);     // seconds
  Bluefruit.Advertising.start(0);               // 0 = forever

  (void)deviceName;
}

}  // namespace