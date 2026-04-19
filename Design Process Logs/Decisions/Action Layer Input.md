# Action Layer Owns Input Modes and Reserved Menu Entry

**Date:** 2026-04-19

## Context
The firmware had begun splitting into hardware input, keymap config, display rendering, and transport layers, but the central application/action layer was still missing. A specific need emerged for an internal menu system that can be entered without extra hardware, while preserving reversible device orientation and future keymap editability.

## Options Considered
- Option A: Bind menu entry to a dedicated hardware button outside the key matrix
- Option B: Reserve a hidden chord on the existing 8-key matrix
- Option C: Bind menu access as a normal mappable action within the keymap

## Decision
The action layer will own device mode switching and menu dispatch. Internal menu entry will initially use a reserved logical-key chord rather than a raw physical button chord or a user-mappable key action.

## Why
The action layer is the correct place to decide whether inputs should be interpreted as normal HID actions or as internal UI/menu controls. Using logical keys for the reserved chord preserves usability across both device orientations. Keeping menu entry outside the user keymap avoids conflicts with editable mappings and guarantees access to firmware-owned functions.

## Implementation Notes
Initial action layer structure includes:
- MODE_NORMAL
- MODE_MENU

Initial reserved menu entry:
- hold logical keys 1–4 for 1 second

Initial menu controls:
- encoder = navigate
- logical key 1 = select
- logical key 2 = back/exit

Normal HID output is suppressed while the reserved chord is being evaluated.

## Risks / Unknowns
A hidden chord may still be less discoverable than a dedicated system button. Exact menu control bindings may need refinement after display integration and real use testing. Future system controls such as BLE pairing, profile switching, or configuration save/load may require additional menu states or a broader system state model.

## Revisit Trigger
Revisit if:
- a dedicated system button is added
- logical chord entry proves awkward in use
- menu functionality expands beyond simple root navigation
- user-editable keymaps introduce conflicts with current assumptions

## Notes
The entry trigger is intentionally decoupled from the menu state machine so it can later be replaced by dedicated hardware, BLE commands, or maintenance/debug triggers without rewriting menu behavior.