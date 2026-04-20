# Display Architecture Split (Mode-Specific Rendering)

**Date:** 2026-04-20

## Context
The initial display system used a single quadrant-based renderer designed for the main operating screen, where key labels are displayed in fixed positions corresponding to physical buttons.

When implementing the preset picker, this renderer was reused, requiring menu content to be forced into the quadrant layout.

## Options Considered
- Option A: Continue using quadrant renderer for all UI modes
- Option B: Introduce separate rendering paths for different UI modes

## Decision
Adopt separate render paths per UI mode:
- Normal mode uses quadrant renderer
- Preset picker uses dedicated list-style renderer
- Future modes (edit/system) will define their own render paths

## Why
Quadrant rendering is tightly coupled to the key-label display model and does not translate well to menu/list interfaces. Forcing menu content into this structure created unnecessary complexity and reduced clarity.

Separating render paths allows each mode to use a layout appropriate to its function.

## Implementation Notes
- Added `displayRenderNormal(...)`
- Added `displayRenderPresetPicker(...)`
- Preset picker renders directly from `ActionLayerState`, not `DisplayState`
- Removed menu usage of quadrant fill logic

## Risks / Unknowns
- Additional render paths may increase display module complexity
- Future modes may require further layout abstraction

## Revisit Trigger
- When implementing preset editor or system menu UI
- If rendering logic becomes duplicated across modes

## Notes
Display module remains centralized; separation is at function level, not file level (for now)