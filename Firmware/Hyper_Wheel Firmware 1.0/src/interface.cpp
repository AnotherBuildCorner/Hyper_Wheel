#include "interface.h"

#include "usb_hid.h"

// Phase one transport router:
// normal actions go to USB HID only for now.
// BLE can be layered in later without changing action layer callers.

void interfaceBegin() {
    usbHidBegin();
}

void interfaceSendKeypress(KeyId key, uint8_t modifiers) {
    usbHidSendKeypress(key, modifiers);
}