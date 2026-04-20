#pragma once

#include <stdint.h>
#include "key_constants.h"

void interfaceBegin();

void interfaceSendKeypress(KeyId key, uint8_t modifiers);