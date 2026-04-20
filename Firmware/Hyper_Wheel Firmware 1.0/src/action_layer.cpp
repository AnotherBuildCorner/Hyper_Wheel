// --- Transport layer. Designed for hanling keypress and encoder events
// PRimarily linking the keypress and encoder events into the menu and HID systems.
#include "action_layer.h"
#include <Arduino.h>
#include "encoder.h"
#include "interface.h"

// =========================
// ROLE: Application/action layer
// DOES:
// - Own current mode (normal vs menu)
// - Detect reserved system/menu chord
// - Route buttons and encoder to HID or menu controls
// DOES NOT:
// - Read GPIO directly
// - Render display directly
// - Persist config to storage yet
// =========================

namespace {

constexpr uint32_t MENU_HOLD_MS = 1000;

// =========================
// Reserved logical chords
// These are firmware-owned chords, not user keymap actions.
// They are defined in logical key space so they remain consistent
// across device orientation.
// =========================

struct ChordBinding {
    const LogicalKeySlot* keys;
    uint8_t keyCount;
    uint32_t holdMs;
};

constexpr LogicalKeySlot MENU_CHORD_KEYS[] = {
    KEY_BOTTOM_LEFT,
    KEY_BOTTOM_RIGHT
};

// Example future chord:
// constexpr LogicalKeySlot SENSITIVITY_CHORD_KEYS[] = {
//     KEY_BOTTOM_1,
//     KEY_BOTTOM_2
// };

constexpr ChordBinding MENU_CHORD = {
    MENU_CHORD_KEYS,
    static_cast<uint8_t>(sizeof(MENU_CHORD_KEYS) / sizeof(MENU_CHORD_KEYS[0])),
    MENU_HOLD_MS
};

// =========================
// Root menu labels
// =========================
constexpr const char* kRootMenuItems[MENU_ACTION_COUNT] = {
    "------", // MENU_ACTION_NONE placeholder
    "PROFIL",
    "PAIRNG",
    "SETING",
    "EXIT"
};

// =========================
// Reusable logical chord detector
// Returns true only if every requested logical key is currently held.
// Extra keys being held do NOT invalidate the chord.
// =========================
bool isLogicalChordHeld(const ActionLayerState& state,
                        const LogicalKeySlot* chordKeys,
                        uint8_t chordCount) {
    if (chordKeys == nullptr || chordCount == 0) {
        return false;
    }

    bool matched[8] = { false };  // NUM_BUTTONS is 8, so this is safe for now

    for (uint8_t phys = 0; phys < NUM_BUTTONS; phys++) {
        PhysicalButtonId button = static_cast<PhysicalButtonId>(phys);
        LogicalKeySlot logical = physicalToLogical(button, state.orientation);

        for (uint8_t i = 0; i < chordCount; i++) {
            if (logical == chordKeys[i]) {
                if (!isButtonDown(button)) {
                    return false;
                }
                matched[i] = true;
            }
        }
    }

    for (uint8_t i = 0; i < chordCount; i++) {
        if (!matched[i]) {
            return false;
        }
    }

    return true;
}

bool isMenuChordHeld(const ActionLayerState& state) {
    return isLogicalChordHeld(state, MENU_CHORD.keys, MENU_CHORD.keyCount);
}

bool shouldSuppressNormalKeysForChord(const ActionLayerState& state) {
    return isMenuChordHeld(state) || state.menuChordArmed;
}

void enterMenu(ActionLayerState& state) {
    Serial.println("[MENU] ENTER MENU");
    state.mode = MODE_MENU;
    state.menuPage = MENU_PAGE_ROOT;
    state.menuIndex = 1;
    state.menuChordArmed = false;
    state.menuChordStartMs = 0;
}

void exitMenu(ActionLayerState& state) {
    Serial.println("[MENU] EXIT MENU");
    state.mode = MODE_NORMAL;
    state.menuChordArmed = false;
    state.menuChordStartMs = 0;
}

void updateMenuChord(ActionLayerState& state) {
    if (state.mode != MODE_NORMAL) {
        state.menuChordArmed = false;
        state.menuChordStartMs = 0;
        return;
    }

    bool chordHeld = isMenuChordHeld(state);

    static bool lastChordHeld = false;
    if (chordHeld != lastChordHeld) {
        Serial.print("[MENU] chordHeld changed: ");
        Serial.println(chordHeld ? "TRUE" : "FALSE");
        lastChordHeld = chordHeld;
    }

    if (chordHeld) {
        if (!state.menuChordArmed) {
            state.menuChordArmed = true;
            state.menuChordStartMs = millis();

            Serial.print("[MENU] chord armed at ms=");
            Serial.println(state.menuChordStartMs);
        } else {
            uint32_t heldMs = millis() - state.menuChordStartMs;

            static uint32_t lastPrintMs = 0;
            if ((millis() - lastPrintMs) > 200) {
                Serial.print("[MENU] chord still held, heldMs=");
                Serial.println(heldMs);
                lastPrintMs = millis();
            }

            if (heldMs >= MENU_CHORD.holdMs) {
                Serial.println("[MENU] hold threshold reached -> entering menu");
                enterMenu(state);
            }
        }
    } else {
        if (state.menuChordArmed) {
            Serial.println("[MENU] chord released before threshold, reset");
        }
        state.menuChordArmed = false;
        state.menuChordStartMs = 0;
    }
}

void handleNormalMode(ActionLayerState& state) {
    updateMenuChord(state);

    if (state.mode != MODE_NORMAL) {
        return;
    }

    if (shouldSuppressNormalKeysForChord(state)) {
        return;
    }

    // Button handling: on press, convert physical -> logical -> active profile action
    for (uint8_t phys = 0; phys < NUM_BUTTONS; phys++) {
        PhysicalButtonId button = static_cast<PhysicalButtonId>(phys);

        if (!wasButtonPressed(button)) {
            continue;
        }

        LogicalKeySlot logical = physicalToLogical(button, state.orientation);

        if (state.config == nullptr) {
            continue;
        }

        if (state.config->activeProfile >= state.config->profileCount) {
            continue;
        }

        const KeyAction& action =
            state.config->profiles[state.config->activeProfile].keys[logical];

        interfaceSendKeypress(action.KeyID, action.modifiers);
    }

    // Encoder handling: keep simple for now
// Encoder handling: profile-driven for encoder 0
    if (encoderHasSteps()) {
        int8_t steps = getPendingEncoderSteps();

        if (state.config != nullptr &&
            state.config->activeProfile < state.config->profileCount) {

            const EncoderBinding& enc =
                state.config->profiles[state.config->activeProfile].encoders[0];

            bool invert = enc.invertDirection;

            while (steps > 0) {
                const KeyAction& action = invert ? enc.counterClockwise : enc.clockwise;
                interfaceSendKeypress(action.KeyID, action.modifiers);
                steps--;
            }

            while (steps < 0) {
                const KeyAction& action = invert ? enc.clockwise : enc.counterClockwise;
                interfaceSendKeypress(action.KeyID, action.modifiers);
                steps++;
            }
        }
    }
}

void handleMenuMode(ActionLayerState& state) {
    if (encoderHasSteps()) {
        int8_t steps = getPendingEncoderSteps();

        while (steps > 0) {
            if (state.menuIndex + 1 < MENU_ACTION_COUNT) {
                state.menuIndex++;
            }
            steps--;
        }

        while (steps < 0) {
            if (state.menuIndex > 1) {
                state.menuIndex--;
            }
            steps++;
        }
    }

    for (uint8_t phys = 0; phys < NUM_BUTTONS; phys++) {
        PhysicalButtonId button = static_cast<PhysicalButtonId>(phys);

        if (!wasButtonPressed(button)) {
            continue;
        }

        LogicalKeySlot logical = physicalToLogical(button, state.orientation);

        // Logical key 1 = select
        if (logical == KEY_TOP_LEFT) {
            switch (state.menuIndex) {
                case MENU_ACTION_EXIT:
                    exitMenu(state);
                    break;

                case MENU_ACTION_PROFILE:
                case MENU_ACTION_PAIRING:
                case MENU_ACTION_SETTINGS:
                default:
                    // Placeholder for next layer of menu logic
                    break;
            }
        }

        // Logical key 2 = back / exit
        if (logical == KEY_TOP_1) {
            exitMenu(state);
        }
    }
}

} // namespace

void actionLayerBegin(ActionLayerState& state,
                      KeymapConfig& config,
                      OrientationMode orientation) {
    state.mode = MODE_NORMAL;
    state.orientation = orientation;
    state.menuChordArmed = false;
    state.menuChordStartMs = 0;
    state.menuPage = MENU_PAGE_ROOT;
    state.menuIndex = 1;
    state.config = &config;
}

void actionLayerSetOrientation(ActionLayerState& state, OrientationMode orientation) {
    state.orientation = orientation;
}

void actionLayerUpdate(ActionLayerState& state) {
    buttonsUpdate();
    debugPrintButtonPresses(state.orientation);
    encoderUpdate();

    if (state.mode == MODE_MENU) {
        handleMenuMode(state);
    } else {
        handleNormalMode(state);
    }
}

bool actionLayerInMenu(const ActionLayerState& state) {
    return (state.mode == MODE_MENU);
}

const char* actionLayerGetMenuTitle(const ActionLayerState& state) {
    switch (state.menuPage) {
        case MENU_PAGE_ROOT:
        default:
            return "MENU";
    }
}

const char* actionLayerGetMenuItemLabel(const ActionLayerState& state, uint8_t index) {
    if (state.menuPage != MENU_PAGE_ROOT) {
        return "------";
    }

    if (index >= 4) {
        return "------";
    }

    uint8_t item = state.menuIndex;

    // Show current item plus nearby entries
    if (index == 0 && item > 1) return kRootMenuItems[item - 1];
    if (index == 1) return kRootMenuItems[item];
    if (index == 2 && (item + 1) < MENU_ACTION_COUNT) return kRootMenuItems[item + 1];

    return "------";
}