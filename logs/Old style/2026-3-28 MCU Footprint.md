# Package Selection Trade Study

## Context

Initial design targeted the nRF52840 QFN48 package due to:
- easier hand assembly (QFN48 vs larger /denser packages)
- reduced routing complexity

## Discovery

Review of Nordic documentation revealed:

- QFN48 package does not support USBD
- VDD and VDDH are internally shorted
- Device limited to normal voltage mode only

## Impact

- Native USB HID is not available in QFN48
- Conflicts with requirement for wired USB fallback
- USB/UART bridge would not provide true HID functionality

## Options Considered

1. Remain on QFN48
   - BLE-only system
   - USB limited to charging and serial debug

2. Add secondary USB-capable MCU
   - Increased complexity
   - Additional firmware coordination required
   - Conflicts with Low Power target

3. Transition to USB-capable nRF52840 package
   - Preserves single-MCU architecture
   - Increased assembly difficulty
   - currently targeting the aQFN73

## Decision

Transition to USB-capable package.

## Notes

This reinforces the importance of verifying package-level feature availability early in the design process.