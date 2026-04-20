#include "main_display.h"

#include "action_layer.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>

static constexpr uint8_t SCREEN_WIDTH = 128;
static constexpr uint8_t SCREEN_HEIGHT = 64;
static constexpr int OLED_RESET = -1;
static constexpr uint8_t OLED_ADDR_A = 0x3D;
static constexpr uint8_t OLED_ADDR_B = 0x3C;

static Adafruit_SSD1306 displayA(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
static Adafruit_SSD1306 displayB(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static DisplayState gLastRenderedNormalState{};
static bool gHasRenderedNormalOnce = false;

static uint8_t gLastRenderedMenuMode = 255;
static uint8_t gLastRenderedHighlight = 255;
static bool gHasRenderedMenuOnce = false;

static bool displayStateEquals(const DisplayState& a, const DisplayState& b) {
    return memcmp(&a, &b, sizeof(DisplayState)) == 0;
}

static void copyLabel(char dest[DISPLAY_LABEL_LEN + 1], const char* src) {
    strncpy(dest, src, DISPLAY_LABEL_LEN);
    dest[DISPLAY_LABEL_LEN] = '\0';
}

void displaySetProfileName(DisplayState& state, const char* text) {
    copyLabel(state.profileName, text);
}

void displaySetLastCommand(DisplayState& state, const char* text) {
    copyLabel(state.lastCommand, text);
}

void displayFillFromKeyLabels(DisplayState& state,
                              const char keyLabels[8][DISPLAY_LABEL_LEN + 1]) {
    copyLabel(state.screenA[0], keyLabels[4]); // Key5
    copyLabel(state.screenA[1], keyLabels[5]); // Key6
    copyLabel(state.screenA[2], keyLabels[0]); // Key1
    copyLabel(state.screenA[3], keyLabels[1]); // Key2

    copyLabel(state.screenB[0], keyLabels[6]); // Key7
    copyLabel(state.screenB[1], keyLabels[7]); // Key8
    copyLabel(state.screenB[2], keyLabels[2]); // Key3
    copyLabel(state.screenB[3], keyLabels[3]); // Key4
}

static void drawQuadrants(Adafruit_SSD1306& d,
                          const char labels[4][DISPLAY_LABEL_LEN + 1],
                          const char* centerText) {
    d.clearDisplay();
    d.setTextColor(SSD1306_WHITE);

    d.setTextSize(1);
    d.setCursor(0, 0);
    d.print(labels[0]);

    d.setCursor(74, 0);
    d.print(labels[1]);

    d.setCursor(0, 54);
    d.print(labels[2]);

    d.setCursor(74, 54);
    d.print(labels[3]);

    d.setTextSize(2);

    int16_t x1, y1;
    uint16_t w, h;
    d.getTextBounds(centerText, 0, 0, &x1, &y1, &w, &h);

    int16_t cx = (SCREEN_WIDTH - w) / 2;
    int16_t cy = (SCREEN_HEIGHT - h) / 2;

    d.setCursor(cx, cy);
    d.print(centerText);

    d.display();
}

static void drawPresetPickerScreenA(Adafruit_SSD1306& d,
                                    uint8_t highlightedPreset,
                                    uint8_t profileCount) {
    d.clearDisplay();
    d.setTextColor(SSD1306_WHITE);

    d.setTextSize(2);
    d.setCursor(0, 0);
    d.print("PRESET");

    d.setTextSize(1);
    d.setCursor(0, 24);
    d.print("Turn: Browse");

    d.setCursor(0, 36);
    d.print("Sys: Select");

    d.setCursor(0, 54);
    d.print(highlightedPreset + 1);
    d.print("/");
    d.print(profileCount);

    d.display();
}

static void drawPresetPickerScreenB(Adafruit_SSD1306& d,
                                    const ActionLayerState& actionState) {
    d.clearDisplay();
    d.setTextColor(SSD1306_WHITE);
    d.setTextSize(1);

    const char* prev = actionLayerGetMenuItemLabel(actionState, 0);
    const char* curr = actionLayerGetMenuItemLabel(actionState, 1);
    const char* next = actionLayerGetMenuItemLabel(actionState, 2);

    uint8_t item = actionState.highlightedPreset;

    d.setCursor(0, 8);
    d.print(" ");
    if (item > 0) {
        d.print(item);
        d.print(": ");
    } else {
        d.print("   ");
    }
    d.print(prev);

    d.drawRect(0, 24, SCREEN_WIDTH, 16, SSD1306_WHITE);
    d.setCursor(2, 28);
    d.print(">");
    d.print(item + 1);
    d.print(": ");
    d.print(curr);

    d.setCursor(0, 48);
    d.print(" ");
    if ((item + 1) < actionState.config->profileCount) {
        d.print(item + 2);
        d.print(": ");
    } else {
        d.print("   ");
    }
    d.print(next);

    d.display();
}

bool displayBegin() {
    Wire.begin();

    if (!displayA.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR_A)) {
        return false;
    }

    if (!displayB.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR_B)) {
        return false;
    }

    displayA.clearDisplay();
    displayA.display();

    displayB.clearDisplay();
    displayB.display();

    return true;
}

void displayRenderNormal(const DisplayState& state) {
    if (gHasRenderedNormalOnce && displayStateEquals(state, gLastRenderedNormalState)) {return;}

    drawQuadrants(displayA, state.screenA, state.profileName);
    drawQuadrants(displayB, state.screenB, state.lastCommand);

    memcpy(&gLastRenderedNormalState, &state, sizeof(DisplayState));
    gHasRenderedNormalOnce = true;
    gHasRenderedMenuOnce = false;
}

void displayRenderPresetPicker(const ActionLayerState& actionState) {
    if (actionState.config == nullptr || actionState.config->profileCount == 0) {
        return;
    }

    if (gHasRenderedMenuOnce &&
        gLastRenderedMenuMode == static_cast<uint8_t>(actionState.mode) &&
        gLastRenderedHighlight == actionState.highlightedPreset) {
        return;
    }

    drawPresetPickerScreenA(displayA,
                            actionState.highlightedPreset,
                            actionState.config->profileCount);

    drawPresetPickerScreenB(displayB, actionState);

    gLastRenderedMenuMode = static_cast<uint8_t>(actionState.mode);
    gLastRenderedHighlight = actionState.highlightedPreset;
    gHasRenderedMenuOnce = true;
}