#include "usb_hid.h"
#include "Adafruit_TinyUSB.h"
#include <Arduino.h>
// Report ID for keyboard
enum {
    RID_KEYBOARD = 1
};

// Standard keyboard HID report descriptor
static uint8_t const desc_hid_report[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(RID_KEYBOARD))
};

static Adafruit_USBD_HID usb_hid;

static uint8_t toUsbKeycode(KeyId key) {
    switch (key) {
        case KEY_A: return HID_KEY_A;
        case KEY_B: return HID_KEY_B;
        case KEY_C: return HID_KEY_C;
        case KEY_D: return HID_KEY_D;
        case KEY_E: return HID_KEY_E;
        case KEY_F: return HID_KEY_F;
        case KEY_G: return HID_KEY_G;
        case KEY_H: return HID_KEY_H;
        case KEY_I: return HID_KEY_I;
        case KEY_J: return HID_KEY_J;
        case KEY_K: return HID_KEY_K;
        case KEY_L: return HID_KEY_L;
        case KEY_M: return HID_KEY_M;
        case KEY_N: return HID_KEY_N;
        case KEY_O: return HID_KEY_O;
        case KEY_P: return HID_KEY_P;
        case KEY_Q: return HID_KEY_Q;
        case KEY_R: return HID_KEY_R;
        case KEY_S: return HID_KEY_S;
        case KEY_T: return HID_KEY_T;
        case KEY_U: return HID_KEY_U;
        case KEY_V: return HID_KEY_V;
        case KEY_W: return HID_KEY_W;
        case KEY_X: return HID_KEY_X;
        case KEY_Y: return HID_KEY_Y;
        case KEY_Z: return HID_KEY_Z;

        case KEY_1: return HID_KEY_1;
        case KEY_2: return HID_KEY_2;
        case KEY_3: return HID_KEY_3;
        case KEY_4: return HID_KEY_4;
        case KEY_5: return HID_KEY_5;
        case KEY_6: return HID_KEY_6;
        case KEY_7: return HID_KEY_7;
        case KEY_8: return HID_KEY_8;
        case KEY_9: return HID_KEY_9;
        case KEY_0: return HID_KEY_0;

        case KEY_ENTER:     return HID_KEY_ENTER;
        case KEY_ESCAPE:    return HID_KEY_ESCAPE;
        case KEY_BACKSPACE: return HID_KEY_BACKSPACE;
        case KEY_TAB:       return HID_KEY_TAB;
        case KEY_SPACE:     return HID_KEY_SPACE;

        case KEY_NUMPAD_0:        return HID_KEY_KEYPAD_0;
        case KEY_NUMPAD_1:        return HID_KEY_KEYPAD_1;
        case KEY_NUMPAD_2:        return HID_KEY_KEYPAD_2;
        case KEY_NUMPAD_3:        return HID_KEY_KEYPAD_3;
        case KEY_NUMPAD_4:        return HID_KEY_KEYPAD_4;
        case KEY_NUMPAD_5:        return HID_KEY_KEYPAD_5;
        case KEY_NUMPAD_6:        return HID_KEY_KEYPAD_6;
        case KEY_NUMPAD_7:        return HID_KEY_KEYPAD_7;
        case KEY_NUMPAD_8:        return HID_KEY_KEYPAD_8;
        case KEY_NUMPAD_9:        return HID_KEY_KEYPAD_9;
        case KEY_NUMPAD_ENTER:    return HID_KEY_KEYPAD_ENTER;
        case KEY_NUMPAD_PLUS:     return HID_KEY_KEYPAD_ADD;
        case KEY_NUMPAD_MINUS:    return HID_KEY_KEYPAD_SUBTRACT;
        case KEY_NUMPAD_MULTIPLY: return HID_KEY_KEYPAD_MULTIPLY;
        case KEY_NUMPAD_DIVIDE:   return HID_KEY_KEYPAD_DIVIDE;
        case KEY_NUMPAD_DECIMAL:  return HID_KEY_KEYPAD_DECIMAL;

        case KEY_MINUS:         return HID_KEY_MINUS;
        case KEY_EQUAL:         return HID_KEY_EQUAL;
        case KEY_LEFT_BRACKET:  return HID_KEY_BRACKET_LEFT;
        case KEY_RIGHT_BRACKET: return HID_KEY_BRACKET_RIGHT;
        case KEY_BACKSLASH:     return HID_KEY_BACKSLASH;
        case KEY_SEMICOLON:     return HID_KEY_SEMICOLON;
        case KEY_APOSTROPHE:    return HID_KEY_APOSTROPHE;
        case KEY_GRAVE:         return HID_KEY_GRAVE;
        case KEY_COMMA:         return HID_KEY_COMMA;
        case KEY_PERIOD:        return HID_KEY_PERIOD;
        case KEY_SLASH:         return HID_KEY_SLASH;

        case KEY_DELETE: return HID_KEY_DELETE;
        case KEY_UP:     return HID_KEY_ARROW_UP;
        case KEY_DOWN:   return HID_KEY_ARROW_DOWN;
        case KEY_LEFT:   return HID_KEY_ARROW_LEFT;
        case KEY_RIGHT:  return HID_KEY_ARROW_RIGHT;

        case KEY_NONE:
        default:
            return 0;
    }
}

static uint8_t toUsbModifiers(uint8_t modifiers) {
    uint8_t out = 0;

    if (modifiers & MOD_CTRL)  out |= KEYBOARD_MODIFIER_LEFTCTRL;
    if (modifiers & MOD_SHIFT) out |= KEYBOARD_MODIFIER_LEFTSHIFT;
    if (modifiers & MOD_ALT)   out |= KEYBOARD_MODIFIER_LEFTALT;
    if (modifiers & MOD_GUI)   out |= KEYBOARD_MODIFIER_LEFTGUI;

    return out;
}

bool usbHidBegin() {


    usb_hid.setBootProtocol(HID_ITF_PROTOCOL_KEYBOARD);
    usb_hid.setPollInterval(2);
    usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
    usb_hid.setStringDescriptor("HyperWheel Keyboard");
    usb_hid.begin();

    delay(200);
    return true;
}


bool usbHidReady() {
    //return TinyUSBDevice.mounted();
    return true;
}

void usbHidReleaseAll() {
    uint8_t emptyKeys[6] = {0, 0, 0, 0, 0, 0};
    usb_hid.keyboardReport(RID_KEYBOARD, 0, emptyKeys);
}

void usbHidSendKeypress(KeyId key, uint8_t modifiers) {

    if (!usbHidReady()) return;

    uint8_t usbCode = toUsbKeycode(key);
    if (usbCode == 0) return;

    uint8_t keycode[6] = {0, 0, 0, 0, 0, 0};
    keycode[0] = usbCode;

    usb_hid.keyboardReport(RID_KEYBOARD, toUsbModifiers(modifiers), keycode);
    delay(3);
        // If we took too long to send the keypress, immediately release to avoid sticking
    usbHidReleaseAll();
}

