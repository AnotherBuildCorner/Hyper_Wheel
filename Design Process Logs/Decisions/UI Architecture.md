# Input Architecture: System Button, Chords, and Preset Interaction Model

**Date:** 2026-04-20

## Context

The device now includes:
- 8 matrix keys (mapped per preset)
- 1 onboard system button (non-key, global control)
- 1+ encoders (profile-driven behavior)
- Chord detection (simultaneous key presses)

The system requires a scalable, low-friction method to access:
- Preset switching
- Preset editing
- Encoder configuration
- Bluetooth host selection
- System-level settings (orientation, pairing, etc.)

Early approaches risked overloading the system button or requiring deep menu navigation for all operations.

## Decision

Adopt a **layered input architecture** with distinct roles:

### 1. System Button → Preset Access
- **Tap:** Toggle between last two active presets
- **Hold:** Open preset picker UI

### 2. Column Chords (Top + Bottom Key in Same Column) → System Portals
Reserved for firmware-level context entry:

| Column | Function           | Activation |
|--------|------------------|------------|
| Col 0  | Preset Editor     | Contextual |
| Col 1  | System Menu       | Tap chord  |
| Col 2  | Bluetooth Menu    | Tap chord  |
| Col 3  | Encoder Tools     | Hold chord |

### 3. Contextual Behavior Inside Preset Picker
When preset picker is open:
- Encoder scrolls presets
- System button confirms selection
- **Preset Editor chord enters edit mode for the currently highlighted preset**

### 4. Encoder Role (Mode-dependent)
- Normal mode: executes profile-defined action
- Menu mode: navigates selections
- Edit mode: modifies values (keycode, modifiers, labels, etc.)
- Hold-chord mode (encoder tools): performs live adjustments (e.g., sensitivity)

## Architectural Model

Inputs are separated into three tiers:

### Tier 1: Direct Actions
- Matrix keys
- Encoder (normal mode)

### Tier 2: Global Access
- System button (preset control)

### Tier 3: Context Entry
- Reserved chords (system-level mode entry)

### Tier 4: Context Execution
- Encoder + keys operate within active mode (menu/edit/system)

## Key Principles

- **Presets are accessed via the system button, not chords**
- **Chords are used for entering system contexts, not performing actions**
- **Editing is context-sensitive and tied to the currently selected preset**
- **Encoder is the primary control within all non-normal modes**
- **Hold-based chords are used for temporary, continuous adjustments**
- **Avoid overloading with multi-tap/double-tap patterns where possible**

## Rationale

This structure:
- Minimizes accidental activation (physical separation of chords)
- Matches frequency of use (presets = frequent, system settings = rare)
- Reduces reliance on timing-based gestures
- Scales cleanly as features are added
- Keeps UI mental model consistent (enter mode → operate within mode)

## Risks / Unknowns

- Exact chord mappings may need refinement based on ergonomics
- Users may require visual cues for chord discovery
- Encoder sensitivity adjustment behavior (temporary vs persistent) is not yet finalized
- Bluetooth usage patterns may influence whether dedicated chord access remains justified

## Revisit Trigger

Revisit after:
- Preset picker and editor flows are fully implemented
- Encoder adjustment behavior is tested in real use
- Bluetooth switching usage patterns are observed

## Notes

- Preset editor is not a top-level mode; it is accessed from within preset context
- Column chords remain reserved for system-level access only
- Future user-programmable chords may be restricted to non-column combinations