# USB-C Connector Placement

**Date:** 2026-04-02

## Context
USB-C connector is located near the MCU and antenna region.  
System is expected to operate primarily in BLE or USB modes, with occasional overlap during charging.

## Decision
Retain current placement for now, with relocation under the wheel section as a fallback option.

## Why
- Layout constraints limit alternative placement options
- Reduced concern about simultaneous high-speed USB and BLE operation
- Current routing remains manageable

## Risks / Unknowns
- Antenna detuning from nearby connector, cable, or ground
- Charger or USB power-path noise during BLE operation while charging
- Mechanical and routing tradeoffs not fully resolved

## Revisit Trigger
- Before final placement lock
- If BLE range or stability is poor during testing (especially while charging)

## Notes
### USB/BLE Coexistence Assumption
System is expected to operate in:
- BLE on battery
- USB wired operation
- USB charging with BLE active

Design does not assume sustained high-speed USB + BLE simultaneously.