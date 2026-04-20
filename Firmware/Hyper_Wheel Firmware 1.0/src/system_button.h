#pragma once

#include <stdint.h>
#include <stdbool.h>

constexpr uint16_t SYSTEM_BUTTON_DEBOUNCE_MS = 20;

void systemButtonBegin();
void systemButtonUpdate();

bool wasSystemButtonPressed();
bool wasSystemButtonReleased();
bool isSystemButtonDown();