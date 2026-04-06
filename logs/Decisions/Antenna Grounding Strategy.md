# Antenna Zone and Grounding Strategy

**Date:** 2026-04-06

## Context
Chip antenna layout showed excessive GND pullback and poor copper merging behavior.  
Initial assumption pointed to custom rule conflicts, but behavior was inconsistent with rule definitions.

## Options Considered
- Option A: Increase global clearance
- Option B: Use custom rules to force zero clearance
- Option C: Adjust zone settings and pad connection locally

## Decision
Use localized zone and pad behavior control:
- Reduce zone clearance to 0.127mm
- Disable thermal relief for antenna GND pads
- Maintain footprint-defined keepout zones

## Why
- Global rules distorted RF geometry unnecessarily  
- Custom rules did not affect zone-level behavior  
- Zone clearance directly controlled copper pullback  
- Pad connection mode controlled actual electrical merging  

## Implementation Notes
- Zone clearance was incorrectly set to 0.5mm  
- Pads 2 and 3 require solid GND connection  
- Keepout zones embedded in footprint ensure portability  

## Risks / Unknowns
- RF performance not validated  
- Geometry deviates slightly from reference layout  
- Sensitive to nearby structures (USB, enclosure, battery)

## Revisit Trigger
- Poor BLE range or instability  
- Layout changes near antenna  
- Transition to production validation

## Notes
This issue originated from zone-level settings, not custom rule definitions.