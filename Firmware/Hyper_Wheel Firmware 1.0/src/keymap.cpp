#include "keymap.h"

KeymapConfig gTestKeymapConfig = {
    1, // version
    0, // activeProfile
    1, // profileCount
    {   // profiles
        {
            "TEST", // name
            {   // keys
                { KEY_A, 0, "K1 A" },
                { KEY_B, 0, "K2 B" },
                { KEY_C, 0, "K3 C" },
                { KEY_D, 0, "K4 D" },
                { KEY_E, 0, "K5 E" },
                { KEY_F, 0, "K6 F" },
                { KEY_G, 0, "K7 G" },
                { KEY_H, 0, "K8 H" }
            },
            {   // encoders
                {
                    { KEY_RIGHT, MOD_SHIFT, "SCROLL" },  // clockwise
                    { KEY_LEFT,  MOD_SHIFT, "" },  // counterClockwise
                    12,                                 // stepsPerDetent
                    0,                                  // acceleration
                    false                               // invertDirection
                },
                {
                    { KEY_NONE, 0, "NONE" },            // clockwise
                    { KEY_NONE, 0, "NONE" },            // counterClockwise
                    12,                                 // stepsPerDetent
                    0,                                  // acceleration
                    false                               // invertDirection
                }
            }
        }
    },
    0 // crc32
};