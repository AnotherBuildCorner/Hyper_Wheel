# Aborting Full PCB Simulation Workflow

**Date:** 2026-04-10

## Context
A possible simulation workflow was investigated for deeper PCB analysis, primarily around PDN, EM, and board-level behavior. The rough open-source path appeared to be KiCad → SALOME → Elmer, with the hope that copper features could be selectively simplified or excluded during model preparation.

The intent was to determine whether a full or near-full board analysis workflow was practical for this design, particularly for a multilayer PCB with dense routing, stitched vias, power distribution, USB, and RF concerns.

## Options Considered
- Option A: Build a full analysis workflow using KiCad exports into SALOME/Elmer and attempt to simulate large portions of the real PCB geometry.
- Option B: Use simplified or targeted analysis only, limiting simulation to specific concerns such as antenna behavior, rough thermal checks, or hand-calculated power integrity.
- Option C: Seek a more dedicated PDN / SI toolchain, accepting that most professional-grade options are commercial and likely impractical for this project.

## Decision
Abort the attempt to pursue full-board FEM / PDN-style analysis as a primary design workflow.

Instead, use targeted analysis only where it provides clear value, and rely on standard PCB design practice, simplified checks, and hand calculations for the rest.

## Why
The open-source simulation path appears to require far more geometry cleanup and abstraction work than is justified for this design.

Key reasons:
- SALOME / Elmer are not PCB-aware tools and do not naturally understand traces, pads, vias, or nets as design objects.
- Selectively excluding traces or simplifying copper after export is not straightforward unless geometry is already separated appropriately.
- A real multilayer board with stitching vias and normal routing detail would require substantial model preparation effort.
- The likely outcome is spending significant time building a simulation pipeline rather than resolving actual board risks.
- Dedicated PDN / SI tools that better match this type of problem are generally commercial and not reasonable for this project budget.

This makes full-board simulation a poor time investment relative to the likely design benefit.

## Implementation Notes
Going forward:
- Use simulation only for narrowly scoped problems where the result meaningfully affects layout decisions.
- RF / antenna behavior remains a possible candidate for targeted simulation.
- Thermal checks may still be worth doing with simplified geometry if needed.
- Power integrity should be handled primarily through layout discipline, trace/via sizing, return path awareness, and hand calculations unless a specific issue emerges.

## Risks / Unknowns
- Some second-order effects may go unmodeled.
- USB and RF behavior will rely more heavily on best practices than numerical verification.
- There is still some chance that a lighter-weight or more specialized free tool could be found later for one subset of the problem.

## Revisit Trigger
Revisit this decision if:
- A specific board issue appears that cannot be explained or resolved by normal layout review.
- A targeted simulation need emerges with clear scope and payoff.
- A practical free or low-cost PCB-aware analysis tool becomes available.
- The design grows substantially in complexity or power density.

## Notes
This is not a rejection of analysis in general. It is a rejection of building a full-board open-source simulation workflow when the tooling overhead outweighs the design value for this project.