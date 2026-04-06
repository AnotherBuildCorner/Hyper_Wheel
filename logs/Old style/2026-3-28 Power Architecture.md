# Power Architecture — Config 4

**Date:** 2026-03-28

## Context
Initial design followed Nordic Config 1 (USB-powered system), but system intent is battery-first operation with USB as optional interface.

## Problem
Config 1 assumes USB as primary supply, conflicting with:
- battery-first operation
- existing power-path charger architecture

## Options Considered
- Config 1 (USB primary)
- Dual-supply approach
- Config 4 (battery-backed system)

## Decision
Transition to Nordic Config 4 power architecture.

## Why
- Aligns with battery-first system behavior
- Avoids conflicting power domains
- Maintains USB functionality
- Enables internal DC/DC for efficiency
- Simplifies power tree

## Implementation Notes
- VDDH powered from charger output (battery-backed rail)
- USB VBUS used only for detection
- DCDCEN0 / DCDCEN1 enabled
- External LDO removed

## Risks / Unknowns
- Must maintain valid VDDH range
- Requires proper low-voltage cutoff handling
- USB routing must remain isolated from system rail

## Revisit Trigger
- During bring-up validation
- If power instability or inefficiency observed