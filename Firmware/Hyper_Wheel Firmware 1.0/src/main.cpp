#include <Arduino.h>
#include <string.h>

#include "buttons.h"
#include "encoder.h"
#include "main_display.h"
#include "action_layer.h"
#include "keymap.h"
#include "interface.h"
#include "config_store.h"
#include "nrf.h"

static DisplayState gDisplayState{};
static ActionLayerState gActionState{};
static DeviceState gDeviceState{};

static bool gDisplayDirty = true;

void enableNfcPinsAsGpio() {
    if (NRF_UICR->NFCPINS != 0) {
        NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen;
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {}

        NRF_UICR->NFCPINS = 0;

        while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {}
        NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren;
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy) {}

        NVIC_SystemReset();
    }
}

static void fillDisplayFromActiveProfile(DisplayState& state, const KeymapConfig& config) {
    char labels[8][DISPLAY_LABEL_LEN + 1] = {};

    const KeymapProfile& profile = config.profiles[config.activeProfile];

    for (uint8_t i = 0; i < 8; i++) {
        strncpy(labels[i], profile.keys[i].label, DISPLAY_LABEL_LEN);
        labels[i][DISPLAY_LABEL_LEN] = '\0';
    }

    displaySetProfileName(state, profile.name);
    displaySetLastCommand(
        state,
        config.profiles[config.activeProfile].encoders[0].clockwise.label
    );
    displayFillFromKeyLabels(state, labels);
}

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

void setup() {
    enableNfcPinsAsGpio();
    Serial.begin(115200);

    //scanI2COnce();
    delay(1000);

    interfaceBegin();
    buttonsBegin();
    systemButtonBegin();
    encoderBegin();


    pinMode(HALL_SENSOR_PIN, INPUT_PULLUP);

    if (!displayBegin()) {
        Serial.println("Display init failed");
        while (1) {
            delay(100);
        }
    }

    configStoreLoadDefaultState(gDeviceState);

    gDeviceState.activePreset = gDefaultKeymapConfig.activeProfile;
    if (gDefaultKeymapConfig.profileCount > 1) {
        gDeviceState.previousPreset =
            (gDefaultKeymapConfig.activeProfile == 0) ? 1 : 0;
    } else {
        gDeviceState.previousPreset = gDefaultKeymapConfig.activeProfile;
    }

    actionLayerBegin(
        gActionState,
        gDefaultKeymapConfig,
        gDeviceState,
        ORIENT_0_DEG
    );

    fillDisplayFromActiveProfile(gDisplayState, gDefaultKeymapConfig);
    displayRenderNormal(gDisplayState);
    gDisplayDirty = false;
}

void loop() {
    bool uiChanged = actionLayerUpdate(gActionState);
    HallTest();

    if (uiChanged) {
        gDisplayDirty = true;
    }

    static InputMode lastMode = MODE_NORMAL;
    if (gActionState.mode != lastMode) {
        gDisplayDirty = true;
        lastMode = gActionState.mode;
    }

    if (gDisplayDirty) {
        if (actionLayerInMenu(gActionState)) {
            displayRenderPresetPicker(gActionState);
        } else {
            fillDisplayFromActiveProfile(gDisplayState, gDefaultKeymapConfig);
            displayRenderNormal(gDisplayState);
        }

        gDisplayDirty = false;
    }

    delay(2);
}


