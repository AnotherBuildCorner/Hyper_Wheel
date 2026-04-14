# Package Selection Trade Study

**Date:** 2026-03-28

## Context
Initial design targeted QFN48 for easier assembly.

## Discovery
QFN48 does not support USB and limits functionality.

## Impact
USB HID not available, conflicts with system requirements.

## Options Considered
- Remain on QFN48
- Add secondary MCU
- Transition to USB-capable package

## Decision
Transition to USB-capable package (aQFN73)

## Why
- Enables native USB HID
- Preserves single-MCU architecture

## Risks / Unknowns
- Increased assembly difficulty

## Notes
Reinforces importance of verifying package capabilities early.
