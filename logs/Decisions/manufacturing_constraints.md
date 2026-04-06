# Manufacturing Constraints - JLCPCB

**Date:** 2026-04-02

## Context
Board must meet JLCPCB standard capabilities (low-cost tier)

## Constraints
- Trace width: 0.127 mm used (min 0.1 mm)
- Spacing: 0.1 mm
- Power traces: 0.2 mm where possible
- No blind/buried vias
- No vias in pad under NRF

## Decision
Designed within standard JLC constraints

## Why
- Keeps cost low
- Avoids advanced fabrication requirements

## Risks / Unknowns
- Tight tolerances → higher failure risk
- Power integrity may be marginal on thin traces
- Routing compromises due to via limitations

## Revisit Trigger
- If thermal or voltage drop issues appear
- If routing becomes overly constrained → consider upgrading stackup
