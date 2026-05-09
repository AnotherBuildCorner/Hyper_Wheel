#include <Arduino.h>
#include <string.h>
#include "buttons.h"

void HallTest(){
    bool x = digitalRead(HALL_SENSOR_PIN);
    static bool lastx = false;
    if (x != lastx) {
        Serial.print("Hall sensor: ");
        Serial.println(x ? "ON" : "OFF");
        lastx = x;
    }

}

#include <Wire.h>

void scanI2COnce() {
    Wire.begin();
    delay(3000);
    Serial.println("I2C scan start");

    uint8_t count = 0;

    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        uint8_t error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("Found device at 0x");
            if (addr < 16) Serial.print("0");
            Serial.println(addr, HEX);
            count++;
        } 
        else if (error == 4) {
            Serial.print("Unknown error at 0x");
            if (addr < 16) Serial.print("0");
            Serial.println(addr, HEX);
        }
    }

    Serial.print("I2C scan complete. Devices found: ");
    Serial.println(count);
}

void AnalogTest() {
    static bool runonce = false;
    if (!runonce) {
        pinMode(CELL_MONITOR_PIN, INPUT);
        runonce = true;
    }
    static uint32_t lasttime = 0;
    if (millis() - lasttime > 1000) {
        int value = analogRead(CELL_MONITOR_PIN);
        float voltage = value * 3.3 / 1023.0;
        Serial.print("Cell monitor: ");
        Serial.print(voltage, 2);
        Serial.println(" V");
        lasttime = millis();
    }

}