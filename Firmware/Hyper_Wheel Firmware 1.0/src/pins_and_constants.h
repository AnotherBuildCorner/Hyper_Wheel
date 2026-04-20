#pragma once
#include <stdint.h>

constexpr uint8_t PIN_BTN_1 = 7;   // D7,  P1.12
constexpr uint8_t PIN_BTN_2 = 8;   // D8,  P1.13
constexpr uint8_t PIN_BTN_3 = 9;   // D9,  P1.14
constexpr uint8_t PIN_BTN_4 = 10;  // D10, P1.15
constexpr uint8_t PIN_BTN_5 = 6;   // D6,  P1.11
constexpr uint8_t PIN_BTN_6 = 3;   // D3,  P0.29
constexpr uint8_t PIN_BTN_7 = 2;   // D2,  P0.28
constexpr uint8_t PIN_BTN_8 = 6;   // D6,  P1.11

constexpr uint8_t PUSH_BTN_PIN = 0; // NFC2 P0.10 (borrowed Cell Monitor Pin for testing)

//constexpr uint8_t HALL_SENSOR_PIN = 5; // NFC1 P0.09 // Move these to Power Handling later.
//constexpr uint8_t CELL_MONITOR_PIN = 0; // D0, P0.02