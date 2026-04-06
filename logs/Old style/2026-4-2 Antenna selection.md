[2.4 GHz Antenna Selection]

Date: 2026-04-02

Context:
Need compact 2.4 GHz antenna for NRF52-based design with limited board space.

Options considered:
- Johanson 2450AT07A0100001T (compact, higher gain, requires keepout)
- Johanson variant allowing ground underneath (lower performance, easier layout)
- Center board variant also considered

Decision:
Selected 2450AT07A0100001T chip antenna

Why:
- Better gain performance
- Smaller footprint
- Acceptable tradeoff vs layout constraints
- Center board model unnaceptably expensive

Risks / Unknowns:
- No RF validation tools (no VNA/spectrum analyzer)
- Matching network tuning not verified
- Layout sensitivity (ground clearance, feed trace, orientation)

Revisit trigger:
- Poor range / connection instability during real-world testing