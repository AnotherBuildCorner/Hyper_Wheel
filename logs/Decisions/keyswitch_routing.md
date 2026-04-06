# Keyswitch Routing Strategy

**Date:** 2026-04-02

## Context
Dense routing around MCU causing fanout congestion

## Options Considered
- Route on top layers (simpler but crowded)
- Push switch matrix to inner layer (Layer 3)

## Decision
Moved keyswitch routing to Layer 3

## Why
- Improves fanout from MCU
- Reduces congestion on outer layers
- Cleaner routing paths

## Risks / Unknowns
- Debug visibility reduced
- Potential coupling/noise if ground reference is poor

## Revisit Trigger
- If signal integrity or scanning issues appear
