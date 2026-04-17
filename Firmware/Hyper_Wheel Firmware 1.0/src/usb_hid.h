#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "key_constants.h"

bool usbHidBegin();
bool usbHidReady();
void usbHidReleaseAll();
void usbHidSendKeypress(KeyId key, uint8_t modifiers);