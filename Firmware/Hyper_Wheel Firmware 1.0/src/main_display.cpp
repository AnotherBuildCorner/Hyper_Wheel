//-- Primary display screen. handling standard in-use view
//further menus will be submodules.

#include "main_display.h"


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
    // Canonical keyLabels indexing:
    // keyLabels[0] = Key1
    // keyLabels[1] = Key2
    // keyLabels[2] = Key3
    // keyLabels[3] = Key4
    // keyLabels[4] = Key5
    // keyLabels[5] = Key6
    // keyLabels[6] = Key7
    // keyLabels[7] = Key8

    // Screen A = 5, 6, 1, 2
    copyLabel(state.screenA[0], keyLabels[4]); // Key5 top-left
    copyLabel(state.screenA[1], keyLabels[5]); // Key6 top-right
    copyLabel(state.screenA[2], keyLabels[0]); // Key1 bottom-left
    copyLabel(state.screenA[3], keyLabels[1]); // Key2 bottom-right

    // Screen B = 7, 8, 3, 4
    copyLabel(state.screenB[0], keyLabels[6]); // Key7 top-left
    copyLabel(state.screenB[1], keyLabels[7]); // Key8 top-right
    copyLabel(state.screenB[2], keyLabels[2]); // Key3 bottom-left
    copyLabel(state.screenB[3], keyLabels[3]); // Key4 bottom-right
}

static void drawQuadrants(Adafruit_SSD1306& d,
                          const char labels[4][DISPLAY_LABEL_LEN + 1],
                          const char* centerText) {
    d.clearDisplay();
    d.setTextSize(1);
    d.setTextColor(SSD1306_WHITE);

    d.setCursor(0, 0);
    d.print(labels[0]);

    d.setCursor(74, 0);
    d.print(labels[1]);

    d.setCursor(0, 54);
    d.print(labels[2]);

    d.setCursor(74, 54);
    d.print(labels[3]);

    int16_t x1, y1;
    uint16_t w, h;
    d.getTextBounds(centerText, 0, 0, &x1, &y1, &w, &h);

    int16_t cx = (SCREEN_WIDTH - w) / 2;
    int16_t cy = (SCREEN_HEIGHT - h) / 2;

    d.setCursor(cx, cy);
    d.print(centerText);

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

void displayRender(const DisplayState& state) {
    drawQuadrants(displayA, state.screenA, state.profileName);
    drawQuadrants(displayB, state.screenB, state.lastCommand);
}