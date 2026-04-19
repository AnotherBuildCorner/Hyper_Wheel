# Core Firmware Architecture Will Be Stabilized Before Menu and BLE Expansion

**Date:** 2026-04-19

## Context
The firmware now includes working pieces for button input, physical-to-logical key mapping, encoder step detection, USB HID transport, display rendering, and an early action layer. BLE service scaffolding also exists, but it is not yet integrated into the active application flow. Menu handling has started inside the action layer, but the normal-mode control path is not yet fully stabilized.

## Options Considered
- Option A: Continue adding menu and BLE features directly into the current scaffold
- Option B: Pause feature expansion and stabilize the architecture around the normal-mode control path first

## Decision
Core architecture stabilization will happen before expanding menu behavior or BLE integration.

## Why
The current codebase has the correct major pieces, but the full normal operating path is not yet cleanly complete. Continuing to add menu and BLE behavior before the normal path is stabilized would increase coupling and debugging difficulty. A stable baseline should exist where hardware input, logical mapping, action lookup, transport routing, and display state all work cleanly in normal mode before additional layers are added.

## Implementation Notes
Near-term architecture target:
- Definitions/config layer
- Hardware input layer
- Transport layer
- UI/render layer
- Application/action routing layer
- Minimal main entry point

Near-term order of work:
1. stabilize normal mode
2. add transport abstraction for USB/BLE routing
3. separate menu behavior into a dedicated menu layer
4. integrate BLE into the common action path
5. add persistence/config editing later

## Risks / Unknowns
Delaying menu and BLE work may feel slower in the short term, but it reduces the chance of architectural churn and hidden coupling. Encoder behavior, display redraw policy, and reserved chord handling may still need refinement during stabilization.

## Revisit Trigger
Revisit once:
- all 8 keys work end-to-end in normal mode
- encoder routing is finalized for normal mode
- action dispatch no longer depends directly on temporary debug or USB-only assumptions

## Notes
The immediate milestone is not feature breadth. It is a clean, reliable normal-mode pipeline that future menu and BLE layers can build on.