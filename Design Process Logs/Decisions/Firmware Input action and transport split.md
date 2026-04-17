# Firmware Input / Action / Transport Split

**Date:** 2026-04-17

## Context
Initial bring-up started from single-file Arduino test sketches for encoder, OLED, and HID behavior. As USB HID, button scanning, display layout, and future BLE support began to converge, keeping all logic in one sketch became increasingly fragile. There was also a risk of coupling the encoder directly to keyboard output and coupling display logic directly to internal modules.

## Options Considered
- Option A:
  Keep most logic in a monolithic sketch and continue extending it with direct calls between buttons, encoder, HID, and OLED code.

- Option B:
  Split firmware into separate modules for inputs, mapping/action resolution, transport, and display rendering, while allowing the encoder to bypass keyboard-specific mapping when needed.

## Decision
Adopt a modular firmware structure where:
- buttons own GPIO scan, debounce, and edge detection
- encoder owns AS5600 reading and motion/step generation
- key/button mappings and encoder bindings resolve into actions
- USB/BLE act as transport layers
- display code renders a prepared display state rather than owning business logic

The encoder may bypass a keyboard-specific keymap when needed, but it should still resolve into the same action layer as the buttons.

## Why
This preserves flexibility without letting the encoder or OLED become side systems with their own incompatible logic paths. It supports:
- keyboard shortcuts for Resolve-style control
- future non-key actions such as scroll or consumer/media control
- future BLE transport without rewriting the core input model
- stable OLED rendering without circular dependencies between modules

## Implementation Notes
Current direction is:
- `buttons.*` for key GPIO handling
- `encoder.*` for AS5600 wheel handling
- `usb_hid.*` for USB keyboard transport
- `display.*` for OLED rendering from a display-state struct
- orientation and label layout handled separately from hardware scan logic

OLED grouping is defined as:
- screen A = keys 5, 6, 1, 2
- screen B = keys 7, 8, 3, 4

## Risks / Unknowns
- HID timing still needs cleanup to remove blocking delays
- BLE transport may impose different timing or report-structure needs
- final action model may need to expand beyond keyboard-only behavior
- shared I2C bus timing with encoder and dual OLEDs may need tuning

## Revisit Trigger
Revisit if:
- encoder needs scroll/media behavior instead of keyboard-only output
- BLE integration forces a different action abstraction
- OLED update timing interferes with input responsiveness
- menu/layer logic begins to blur module boundaries again

## Notes
The immediate proof point for this architecture is successful integration of:
1. dual OLED runtime screen
2. 8-button scan path
3. encoder and button actions through one common dispatch path