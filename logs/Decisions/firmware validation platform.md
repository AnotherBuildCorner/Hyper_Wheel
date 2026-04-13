# Firmware Validation Platform Architecture

**Date:** 2026-04-12

## Context
The original design aimed to integrate full system functionality on a custom nRF52840 PCB, including power management (LiPo charging, load switches), multiple I2C buses, and low-power optimization.

This introduced significant complexity in layout and bring-up:
- High GPIO demand exceeding available pins
- Increased routing complexity (multi-layer, power domains)
- Higher risk during initial firmware validation

A faster iteration path was needed to validate firmware, UI, and core functionality independently.

---

## Options Considered

- Option A: Continue full custom board  
  - Pros:
    - Direct path to final hardware
    - No migration required  
  - Cons:
    - Slower iteration
    - Higher debugging complexity
    - Firmware blocked by hardware issues

- Option B: Simplified dev board using XIAO nRF52840  
  - Pros:
    - Rapid bring-up
    - Reduced layout complexity (2-layer)
    - Known-good RF and power subsystem  
  - Cons:
    - Limited GPIO
    - Missing system features
    - Requires later migration

---

## Decision
Adopt a dedicated firmware validation platform using the XIAO nRF52840.

This platform:
- Prioritizes firmware and UI validation
- Uses a simplified 2-layer PCB
- Removes non-critical systems (charger, load switching, secondary I2C)
- Preserves physical layout for key interfaces (switches and displays)

---

## Why
Iteration speed is the primary constraint.

Separating firmware from full hardware:
- Enables independent firmware development
- Reduces compounded debugging complexity
- Allows faster iteration cycles

---

## Implementation Notes

- Removed due to GPIO limits:
  - Load switches
  - Secondary I2C bus

- Converted to 2-layer PCB
- Simplified power distribution (rear-layer zones)
- Updated components to 0603 where practical
- Added bypass jumpers for LiPo anti-drain circuit
- Maintained placement for:
  - Key switches
  - OLED displays
- Added board extension to accommodate XIAO module

Planned:
- AS5600 + hall sensor breakout boards
  - Include I2C pull-ups
  - Add protection components for reuse

---

## Risks / Unknowns

- Firmware may depend on simplified hardware assumptions
- Migration risks:
  - Power sequencing differences
  - I2C loading changes
  - GPIO reassignment conflicts
- Load switching and low-power behavior not validated
- Breakout boards may not reflect final signal integrity

---

## Revisit Trigger

- Firmware reaches stable baseline:
  - Input handling
  - Display control
  - Encoder behavior

- Need to validate:
  - Power system
  - Multi-device I2C
  - Battery management

---

## Notes

Firmware must use assignable / abstracted pin definitions.

Failure to enforce this will significantly increase migration difficulty.