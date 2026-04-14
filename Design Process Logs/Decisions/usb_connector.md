# USB-C Connector Placement

**Date:** 2026-04-02

## Context
USB-C connector is relatively near MCU / antenna region. System is expected to use either BLE or USB in most operating modes, with charging + BLE as a possible overlap case.

## Decision
Current placement retained for now; relocation under wheel section remains a fallback option.

## Why
- Layout constraints
- Reduced concern about simultaneous high USB data activity and BLE operation

## Risks / Unknowns
- Antenna detuning from nearby connector/cable/ground
- Charger or USB power-path noise during BLE use while charging
- Mechanical and routing tradeoffs not fully resolved

## Revisit Trigger
- Before final placement lock
- If BLE range or stability is poor in testing, especially while charging

## Notes

### USB/BLE Coexistence Assumption
System is expected to operate primarily in one of three modes:
- BLE on battery
- USB wired operation
- USB charging with BLE active

Design does not assume sustained high-speed USB data activity concurrent with BLE.
This reduces expected coexistence issues, but antenna placement and USB region layout still require care.
