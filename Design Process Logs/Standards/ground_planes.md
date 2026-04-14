# Ground Plane Strategy

**Date:** 2026-04-02

## Context
4-layer board design.

## Strategy
Use dedicated internal ground planes with outer layer fills.

## Structure
- Layers 2 and 3: solid ground
- Layers 1 and 4: signal + partial fill

## Rationale
- Improves return paths
- Reduces noise
- Supports RF + digital stability

## Risks / Tradeoffs
- Improper stitching → fragmented ground
- Antenna keepout violations

## Revisit Trigger
- Final layout review
