# I2C Display Architecture

**Date:** 2026-04-02

## Context
Want UI feedback for key assignments

## Options Considered
- Single OLED
- Dual OLEDs on same bus
- Fully integrated display vs module
- Run each screen on separate bus, shared with one AS5600 each

## Decision
- Two I2C OLED modules on same bus [Tentative]
- Using prebuilt modules instead of raw OLED integration

## Why
- Faster implementation
- Library compatibility
- Visual layout (corner displays)

## Risks / Unknowns
- I2C address conflicts (must be configurable)
- Bus capacitance (especially if wiring expands)
- Limited refresh rate if both update frequently

## Revisit Trigger
- If UI lag or bus instability appears

---

# OLED Integration Strategy

## Context
Choice between integrating OLED directly vs using module

## Decision
Use I2C OLED module footprint instead of full integration

## Why
- Avoids driver/library complexity
- Reduces firmware risk
- Faster iteration

## Risks / Unknowns
- Larger footprint
- Less control over hardware optimization

## Revisit Trigger
- If moving toward production or space becomes critical
