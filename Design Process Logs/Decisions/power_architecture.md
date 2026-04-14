# Power Architecture Update — Config 1 → Config 4

**Date:** 2026-03-28

## Context
Initial power architecture aligned with Nordic Config 1, assuming USB as primary supply.

## Problem
Config 1 conflicts with battery-first system design and existing charger architecture.

## Observations
Config 4 supports battery-backed operation and USB functionality.

## Decision
Transition to Config 4 power architecture.

## Why
- Aligns with battery-first behavior
- Avoids conflicting power domains
- Maintains USB functionality
- Improves efficiency via DC/DC

## Implementation Notes
- VDDH from charger output
- USB VBUS used for detection only
- DCDCEN0 / DCDCEN1 enabled
- External LDO removed

## Risks / Unknowns
- Must ensure valid VDDH range
- Requires proper low-voltage cutoff handling
- Careful USB routing required

## Revisit Trigger
- During bring-up validation
