# Event-Driven Display Update Model

**Date:** 2026-04-20

## Context
The display was originally updated continuously in the main loop, calling `displayRender()` every iteration.

This caused performance issues:
- OLED updates over I2C consumed significant time
- Encoder responsiveness degraded due to shared I2C bus contention

## Options Considered
- Option A: Continue constant redraw loop
- Option B: Throttle redraw using timing delays
- Option C: Move to event-driven redraw model

## Decision
Adopt event-driven redraw model using a dirty flag system.

## Why
Continuous redraw was unnecessary and interfered with input responsiveness. Most UI frames do not change every loop iteration.

Event-driven updates ensure:
- redraw only occurs when state changes
- I2C bandwidth is preserved
- encoder input remains responsive

## Implementation Notes
- `actionLayerUpdate()` returns a `uiChanged` flag
- Main loop sets `gDisplayDirty` when UI changes
- Mode transitions also force redraw
- Display renders only when `gDisplayDirty == true`

## Risks / Unknowns
- Missed dirty flag updates could cause stale display
- Mode transitions must always trigger redraw

## Revisit Trigger
- If UI becomes more dynamic (animations, live telemetry)
- If missed redraw bugs appear

## Notes
Menu-level caching was removed or minimized due to conflict with external dirty flag system