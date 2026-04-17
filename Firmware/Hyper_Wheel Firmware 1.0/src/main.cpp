#include <Arduino.h>
#include "encoder.h"

static EncoderBindings gEncoderBindings = {
    { KEY_RIGHT,  MOD_SHIFT },   // CW
    { KEY_LEFT,  MOD_SHIFT }    // CCW
};

void setup() {
    Serial.begin(115200);
    delay(1000);
    usbHidBegin();

    if (encoderBegin()) {
        Serial.println("Encoder init OK");
    } else {
        Serial.println("Encoder init FAILED");
    }
}

void loop() {
    encoderUpdate();

if (encoderHasSteps()) {
    int8_t steps = getPendingEncoderSteps();

    if (steps > 0) {
        for (int8_t i = 0; i < steps; i++) {
            usbHidSendKeypress(gEncoderBindings.clockwise.keyId,
                               gEncoderBindings.clockwise.modifiers);
        }
    } else if (steps < 0) {
        for (int8_t i = 0; i < -steps; i++) {
            usbHidSendKeypress(gEncoderBindings.counterClockwise.keyId,
                               gEncoderBindings.counterClockwise.modifiers);
        }
    }
}

        Serial.print("  angle=");
        Serial.print(getEncoderAngle());
        Serial.print("  diff=");
        Serial.print(getEncoderDiff());
        Serial.print("  count=");
        Serial.println(getEncoderCount());
    

    delay(2);
}

