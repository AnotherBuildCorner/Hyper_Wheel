// Core encoder code, simple interface for reading the AS5600 and converting to a step-based model for the rest of the codebase.

#include "encoder.h"

#include <Wire.h>
#include <AS5600.h>

// -----------------------------------------------------------------------------
// I2C / AS5600
// -----------------------------------------------------------------------------
static const uint8_t EncoderAADDR = 0x40;
static const uint8_t EncoderBADDR = 0x36;

static AS5600 encoder(EncoderAADDR);
static AS5600 encoderB(EncoderBADDR);

// -----------------------------------------------------------------------------
// Encoder tuning
// -----------------------------------------------------------------------------
static constexpr int COUNTS_PER_REV = 4096;
static constexpr int DEAD_BAND      = 0;
static uint16_t gStepCounts = 12;
static constexpr int MOTION_THRESHOLD = 1;
static constexpr uint16_t SETTLE_MS = 40;
static uint32_t gLastMotionMs = 0;

// -----------------------------------------------------------------------------
// Internal state
// -----------------------------------------------------------------------------
static int16_t gAccum = 0;
static uint16_t gLastAngle = 0;
static int16_t gLastDiff = 0;
static long gEncoderCount = 0;

//static volatile EncoderEventType gPendingEvent = ENC_EVENT_NONE;
static volatile int8_t gPendingSteps = 0;
// -----------------------------------------------------------------------------
// Helper: wrap-aware difference
// -----------------------------------------------------------------------------
static inline int16_t wrapDiff(uint16_t now, uint16_t prev) {
    int16_t d = static_cast<int16_t>(now) - static_cast<int16_t>(prev);

    if (d >  (COUNTS_PER_REV / 2)) d -= COUNTS_PER_REV;
    if (d < -(COUNTS_PER_REV / 2)) d += COUNTS_PER_REV;

    return d;
}

// -----------------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------------

//-- Call encoder once, established connection and set initial state.

bool encoderBegin() {
    Wire.begin();
    gLastMotionMs = millis();

    encoderA.begin();
    encoderA.setDirection(AS5600_CLOCK_WISE);

    gLastAngle = encoder.readAngle();
    gLastDiff = 0;
    gAccum = 0;
    gEncoderCount = 0;
    //gPendingEvent = ENC_EVENT_NONE;
    gPendingSteps = 0;

    return true;
}
// Update encoder state, call periodically from loop. This will read the current angle, compare to the last angle, and if it exceeds thresholds, update the pending event and step count for the rest of the system to consume.
// dead band in steps is probably nonsense, May switch to a single raw count instead. So it's not tied to sensitivity.
void encoderUpdate() {
    uint32_t now = millis();

    uint16_t angle = encoderA.readAngle();
    int16_t diff = wrapDiff(angle, gLastAngle);

    gLastAngle = angle;
    gLastDiff = diff;

    // Only count this as real motion if it exceeds threshold
    if (abs(diff) >= MOTION_THRESHOLD) {
        gLastMotionMs = now;
        gAccum += diff;
        Serial.println("Encoder Motion Thresh");
    } else {
        // If we have been still for a little while, flush leftover residue
        if ((now - gLastMotionMs) > SETTLE_MS) {
            gAccum = 0;
        }
        return;
    }

    while (abs(gAccum) >= gStepCounts) {
        if (gAccum > 0) {
            gEncoderCount++;
            gAccum -= gStepCounts;

            if (gPendingSteps < 20) {
                gPendingSteps++;
            }
        } else {
            gEncoderCount--;
            gAccum += gStepCounts;

            if (gPendingSteps > -20) {
                gPendingSteps--;
            }
        }
    }
}

void encoderSetStepCounts(uint16_t stepCounts) {
    if (stepCounts == 0) {
        stepCounts = 1;
    }
    gStepCounts = stepCounts;
}

bool encoderHasSteps() {
    return (gPendingSteps != 0);
}

int8_t getPendingEncoderSteps() {
    int8_t steps = gPendingSteps;
    gPendingSteps = 0;
    return steps;
}


long getEncoderCount() {
    return gEncoderCount;
}

uint16_t getEncoderAngle() {
    return gLastAngle;
}

int16_t getEncoderDiff() {
    return gLastDiff;
}