# Rear-Mounted Component Strategy for Final Board

**Date:** 2026-04-24

## Context
Rev A XIAO board inspection showed that front-side component placement creates mechanical and accessibility constraints. The keyswitch sockets were misunderstood as front-mounted parts when they are actually rear-mounted for front-facing switches. The OLED module pinout also created a layout failure due to mismatch between reference symbol assumptions and the final module used.

## Options Considered
- Option A: Keep most components front-mounted.
- Option B: Rear-mount most SMT components except through-hole/user-facing items.
- Option C: Rear-mount most SMT components and replace OLED module with onboard SSD1306/display circuitry.

## Current Direction
Rear-mounting most non-through-hole components is under consideration for the final board.

## Why
Rear mounting may improve test pad access, reduce front-side crowding, and leave the front face primarily for user-interface elements: keys, display, wheel, and any required indicators.

## Implementation Notes
- Keep THT/mechanical interface items front-oriented where needed.
- Re-check hotswap socket footprint side/orientation explicitly.
- Verify OLED/display pinout from the exact final part, not a generic module reference.
- Charge LED may need a light pipe, edge-facing LED, or alternate visible indicator strategy.
- Onboard SSD1306/display integration may change this decision.

## Risks / Unknowns
- Rear-mounted parts may complicate assembly or enclosure clearance.
- Charge indicator visibility may become awkward.
- Display strategy is not yet settled.
- Test pad access improves only if enclosure/mechanical design leaves the rear reachable.

## Revisit Trigger
Revisit before Rev B/full-board routing is locked, especially after deciding between OLED module and onboard SSD1306/display implementation.