# Passive Component Sizing Strategy

**Date:** 2026-03-26

## Context
Reference design uses 0201 components extensively.

## Strategy
Divide components into categories based on function.

## Categories / Rules
- RF-Critical (retain 0201)
- Timing-Critical (0402 consideration)
- General Passives (0402)
- Bulk Components (0603)

## Rationale
- Maintain RF integrity
- Improve assembly success rate
- Reduce placement difficulty

## Notes
Component size affects parasitics at RF frequencies.
