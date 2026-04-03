# Crystal Footprint and Package Interpretation

## Context

Initial schematic review suggested ambiguity in crystal pin count based on symbol representation.

## Observation

- Schematic representation appeared to imply a third connection
- BOM entry only specified package size (XTAL SMD 2016)
- No explicit pin count provided

## Resolution

Review of reference PCB layout shows:

- 4-pad crystal footprint (2 active + 2 grounded case pads)
- Two pads routed to XC1 / XC2
- Two pads tied to ground plane

## Decision

Use 4-pad 2016 crystal footprint with grounded case.

## Notes

- Schematic symbols can be misleading when nodes are shared visually
- PCB layout is the authoritative reference for footprint selection
- Grounded crystal cases improve EMI and stability in RF designs