#include "keymap.h"
#include <string.h>




KeymapConfig gDefaultKeymapConfig = {
    1, // version
    1, // activeProfile
    4, // profileCount
    {
        //------
        { // Profile 0
            "Retime",
            {
                { KEY_0,MOD_CTRL, "Retime"},
                { KEY_DELETE, 0, "RipDel" },
                { KEY_SPACE, 0, "Play" },
                { KEY_0, 0, "Frame" },
                { KEY_G, MOD_CTRL|MOD_ALT, "Grab" },
                { KEY_NUMPAD_3, 0, "> Cut" },
                { KEY_Y, MOD_CTRL|MOD_ALT, "BWD-A" },
                { KEY_Y, MOD_ALT, "FWD-A" }
            },
            {
                {
                    { KEY_RIGHT, 0, "SCROLL" },
                    { KEY_LEFT,  0, "" },
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
                }}},
//-----------------------------------------
        // Profile 1
        {
            "DAVINC",
            {
                { KEY_Z, MOD_CTRL, "UNDO" },
                { KEY_DELETE, 0, "RipDel" },
                { KEY_SPACE, 0, "Play" },
                { KEY_B, 0, "Break" },
                { KEY_NUMPAD_1, 0, "< Cut" },
                { KEY_NUMPAD_3, 0, "> Cut" },
                { KEY_Y, MOD_CTRL|MOD_ALT, "BWD-A" },
                { KEY_Y, MOD_ALT, "FWD-A" }
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

        // Profile 2
        {
            "DV2",
            {
                { KEY_1, 0, "K1 F1" },
                { KEY_2, 0, "K2 F2" },
                { KEY_3, 0, "K3 F3" },
                { KEY_4, 0, "K4 F4" },
                { KEY_5, 0, "K5 F5" },
                { KEY_6, 0, "K6 F6" },
                { KEY_7, 0, "K7 F7" },
                { KEY_8, 0, "K8 F8" }
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
        },
    
//---------------------------------------------
        { // Profile 0
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
                }}},
            
            
            
            },
    0 // crc
};

void copyDefaultKeymapConfig(KeymapConfig& outConfig) {
    memcpy(&outConfig, &gDefaultKeymapConfig, sizeof(KeymapConfig));
}