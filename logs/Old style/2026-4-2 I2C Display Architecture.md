[I2C Display Architecture]

Context:
Want UI feedback for key assignments

Options:
- Single OLED
- Dual OLEDs on same bus
- Fully integrated display vs module
- Run each Screen on separate bus, shared with one AS5600 each

Decision:
- Two I2C OLED modules on same bus [Tenative]
- Using prebuilt modules instead of raw OLED integration

Why:
- Faster implementation
- Library compatibility
- Visual layout (corner displays)


Risks:
- I2C address conflicts (must be configurable)
- Bus capacitance (especially if wiring expands)
- Limited refresh rate if both update frequently

Revisit:
- If UI lag or bus instability appears

[OLED Integration Strategy]

Context:
Choice between integrating OLED directly vs using module

Decision:
Use I2C OLED module footprint instead of full integration

Why:
- Avoids driver/library complexity
- Reduces firmware risk
- Faster iteration

Risks:
- Larger footprint
- Less control over hardware optimization

Revisit:
- If moving toward production or space becomes critical