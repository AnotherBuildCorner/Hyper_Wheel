#include "keymap.h"
#include <string.h>




KeymapConfig gDefaultKeymapConfig = {
    1, // version
    1, // activeProfile
    2, // profileCount

    {
        // Profile 0
        {
            "BASE",
            {
                { KEY_A, 0, "K1 A" },
                { KEY_B, 0, "K2 B" },
                { KEY_C, 0, "K3 C" },
                { KEY_D, 0, "K4 D" },
                { KEY_E, 0, "K5 E" },
                { KEY_F, 0, "K6 F" },
                { KEY_G, 0, "K7 G" },
                { KEY_H, 0, "K8 H" }
            },
            {
                {
                    { KEY_RIGHT, MOD_SHIFT, "SCROLL" },
                    { KEY_LEFT,  MOD_SHIFT, "" },
                    12,
                    0,
                    false
                },
                {
                    { KEY_NONE, 0, "NONE" },
                    { KEY_NONE, 0, "NONE" },
                    12,
                    0,
                    false
                }
            }
        },

        // Profile 1
        {
            "ALT",
            {
                { KEY_1, 0, "K1 1" },
                { KEY_2, 0, "K2 2" },
                { KEY_3, 0, "K3 3" },
                { KEY_4, 0, "K4 4" },
                { KEY_5, 0, "K5 5" },
                { KEY_6, 0, "K6 6" },
                { KEY_7, 0, "K7 7" },
                { KEY_8, 0, "K8 8" }
            },
            {
                {
                    { KEY_RIGHT,  0, "Scroll" }, //CW
                    { KEY_LEFT, 0, "DOWN" }, //CCW
                    8,
                    0,
                    false
                },
                {
                    { KEY_NONE, 0, "NONE" },
                    { KEY_NONE, 0, "NONE" },
                    12,
                    0,
                    false
                }
            }
        }
    },

    0 // crc
};

void copyDefaultKeymapConfig(KeymapConfig& outConfig) {
    memcpy(&outConfig, &gDefaultKeymapConfig, sizeof(KeymapConfig));
}