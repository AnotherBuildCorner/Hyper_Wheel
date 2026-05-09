#include "encoder.h"

#include <Wire.h>
#include <AS5600.h>

// -----------------------------------------------------------------------------
// Encoder instance state
// -----------------------------------------------------------------------------
static bool EnableAuxEnc = false;
static bool SerialPrint = true;

struct EncoderState {
    AS5600L dev;

    uint8_t address;

    uint16_t stepCounts = 12;
    uint16_t lastAngle = 0;
    int16_t lastDiff = 0;
    int16_t accum = 0;
    long count = 0;

    uint32_t lastMotionMs = 0;
    volatile int8_t pendingSteps = 0;

    EncoderState(uint8_t addr)
        : address(addr) {}
};

// -----------------------------------------------------------------------------
// I2C / AS5600L
// -----------------------------------------------------------------------------

static constexpr uint8_t EncoderAADDR = 0x40;
static constexpr uint8_t EncoderBADDR = 0x36; // placeholder until second AS5600L is real

static EncoderState encoderA(EncoderAADDR);
static EncoderState encoderB(EncoderBADDR);

// -----------------------------------------------------------------------------
// Encoder tuning
// -----------------------------------------------------------------------------

static constexpr int COUNTS_PER_REV = 4096;
static constexpr int MOTION_THRESHOLD = 1;
static constexpr uint16_t SETTLE_MS = 40;
static constexpr int8_t MAX_PENDING_STEPS = 20;

// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------

static EncoderState& getEncoderState(EncoderId id) {
    switch (id) {
        case ENCODER_A:
            return encoderA;

        case ENCODER_B:
            return encoderB;

        default:
            return encoderA;
    }
}

static inline int16_t wrapDiff(uint16_t now, uint16_t prev) {
    int16_t d = static_cast<int16_t>(now) - static_cast<int16_t>(prev);

    if (d >  (COUNTS_PER_REV / 2)) d -= COUNTS_PER_REV;
    if (d < -(COUNTS_PER_REV / 2)) d += COUNTS_PER_REV;

    return d;
}

static bool beginEncoder(EncoderState& enc) {
    enc.dev.begin();
    enc.dev.setAddress(enc.address);
    Serial.print("Encoder ");
    Serial.print(enc.address, HEX);
    Serial.println(" initialized.");

    enc.dev.setDirection(AS5600_CLOCK_WISE);

    enc.lastAngle = enc.dev.readAngle();
    enc.lastDiff = 0;
    enc.accum = 0;
    enc.count = 0;
    enc.pendingSteps = 0;
    enc.lastMotionMs = millis();

    return true;
}

static void updateEncoder(EncoderState& enc) {
    uint32_t now = millis();

    uint16_t angle = enc.dev.readAngle();
    int16_t diff = wrapDiff(angle, enc.lastAngle);



    enc.lastAngle = angle;
    enc.lastDiff = diff;

    if (abs(diff) >= MOTION_THRESHOLD) {
        enc.lastMotionMs = now;
        enc.accum += diff;
    } else {
        if ((now - enc.lastMotionMs) > SETTLE_MS) {
            enc.accum = 0;
        }
        return;
    }

    while (abs(enc.accum) >= enc.stepCounts) {
        if (enc.accum > 0) {
            enc.count++;
            enc.accum -= enc.stepCounts;

            if (enc.pendingSteps < MAX_PENDING_STEPS) {
                enc.pendingSteps++;
            }
        } else {
            enc.count--;
            enc.accum += enc.stepCounts;

            if (enc.pendingSteps > -MAX_PENDING_STEPS) {
                enc.pendingSteps--;
            }
            
        }

    }
}

static void setEncoderStepCounts(EncoderState& enc, uint16_t stepCounts) {
    if (stepCounts == 0) {
        stepCounts = 1;
    }

    enc.stepCounts = stepCounts;
}

static int8_t consumePendingSteps(EncoderState& enc) {
    int8_t steps = enc.pendingSteps;
    enc.pendingSteps = 0;
    return steps;
}

// -----------------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------------

bool encoderBegin() {
    Wire.begin();

    bool okA = beginEncoder(encoderA);
    if(EnableAuxEnc){
        bool okB = beginEncoder(encoderB);
    }

    // Leave this disabled until the second real AS5600L exists.
    // bool okB = beginEncoder(encoderB);

    return okA;
}

void encoderUpdate() {
    updateEncoder(encoderA);
    if(EnableAuxEnc){updateEncoder(encoderB);}

    // Leave disabled until second real encoder exists.
    // updateEncoder(encoderB);
}

int8_t getPendingEncoderSteps(EncoderId id) {
    return consumePendingSteps(getEncoderState(id));
}

bool encoderHasSteps(EncoderId id) {
    return getEncoderState(id).pendingSteps != 0;
}

long getEncoderCount(EncoderId id) {
    return getEncoderState(id).count;
}

uint16_t getEncoderAngle(EncoderId id) {
    return getEncoderState(id).lastAngle;
}

int16_t getEncoderDiff(EncoderId id) {
    return getEncoderState(id).lastDiff;
}

void encoderSetStepCounts(EncoderId id, uint16_t stepCounts) {
    setEncoderStepCounts(getEncoderState(id), stepCounts);
}