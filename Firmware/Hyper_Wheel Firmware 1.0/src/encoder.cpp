// Core encoder code, simple interface for reading the AS5600 and converting to a step-based model for the rest of the codebase.

#include "encoder.h"

#include <Wire.h>
#include <AS5600.h>

// -----------------------------------------------------------------------------
// I2C / AS5600
// -----------------------------------------------------------------------------
static AS5600 encoder;

// -----------------------------------------------------------------------------
// Encoder tuning
// -----------------------------------------------------------------------------
static constexpr int COUNTS_PER_REV = 4096;
static constexpr int DEAD_BAND      = 0;
static constexpr int STEP_COUNTS    = 12;
static constexpr int MOTION_THRESHOLD = 0;
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
bool encoderBegin() {
    Wire.begin();
    gLastMotionMs = millis();

    encoder.begin();
    encoder.setDirection(AS5600_CLOCK_WISE);

    gLastAngle = encoder.readAngle();
    gLastDiff = 0;
    gAccum = 0;
    gEncoderCount = 0;
    //gPendingEvent = ENC_EVENT_NONE;
    gPendingSteps = 0;

    return true;
}

void encoderUpdate() {
    uint32_t now = millis();

    uint16_t angle = encoder.readAngle();
    int16_t diff = wrapDiff(angle, gLastAngle);

    gLastAngle = angle;
    gLastDiff = diff;

    // Only count this as real motion if it exceeds threshold
    if (abs(diff) >= MOTION_THRESHOLD) {
        gLastMotionMs = now;
        gAccum += diff;
    } else {
        // If we have been still for a little while, flush leftover residue
        if ((now - gLastMotionMs) > SETTLE_MS) {
            gAccum = 0;
        }
        return;
    }

    while (abs(gAccum) >= STEP_COUNTS) {
        if (gAccum > 0) {
            gEncoderCount++;
            gAccum -= STEP_COUNTS;

            if (gPendingSteps < 20) {
                gPendingSteps++;
            }
        } else {
            gEncoderCount--;
            gAccum += STEP_COUNTS;

            if (gPendingSteps > -20) {
                gPendingSteps--;
            }
        }
    }
}

/*bool encoderHasEvent() {
    return (gPendingEvent != ENC_EVENT_NONE);
}*/

bool encoderHasSteps() {
    return (gPendingSteps != 0);
}

int8_t getPendingEncoderSteps() {
    int8_t steps = gPendingSteps;
    gPendingSteps = 0;
    return steps;
}

/*EncoderEventType getEncoderEvent() {
    EncoderEventType event = gPendingEvent;
    gPendingEvent = ENC_EVENT_NONE;
    return event;
}*/

Action getEncoderAction() {
    Action action{};
    EncoderEventType event = getEncoderEvent();

    switch (event) {
        case ENC_EVENT_CW:
            action.type = ACTION_CONSUMER;
            action.code = 1;   // placeholder for volume up
            action.value = 1;
            action.modifiers = 0;
            break;

        case ENC_EVENT_CCW:
            action.type = ACTION_CONSUMER;
            action.code = 2;   // placeholder for volume down
            action.value = -1;
            action.modifiers = 0;
            break;

        case ENC_EVENT_NONE:
        default:
            action.type = ACTION_NONE;
            action.code = 0;
            action.value = 0;
            action.modifiers = 0;
            break;
    }

    return action;
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