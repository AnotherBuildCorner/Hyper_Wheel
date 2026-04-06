# Passive Component Sizing Strategy

**Date:** 2026-03-26

## Context
Reference design uses 0201 components extensively, which are difficult to hand assemble.

## Strategy
Use mixed component sizing based on functional requirements.

## Categories / Rules
- RF-Critical → 0201
- Timing-Critical → 0402 (tight placement)
- General Passives → 0402
- Bulk Components (≥1 µF) → 0603

## Rationale
- Maintain RF integrity where required
- Improve assembly success rate
- Reduce placement difficulty

## Practical Impact
- Easier assembly process
- Slight increase in layout space requirements
- Minimal RF impact if applied correctly

## Boundaries
- RF path components must remain small and tightly placed
- Non-RF sections can tolerate larger components

## Risks / Tradeoffs
- Larger components introduce parasitics
- Crystal performance sensitivity

## Revisit Trigger
- If RF issues arise
- If assembly proves problematic