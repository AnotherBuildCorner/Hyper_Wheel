# Hyper Wheel

A battery-powered BLE + USB HID input device featuring a magnetic encoder, modular input design, and low-power architecture built around the nRF52840.

---

## Overview

Hyper Wheel is a custom human interface device designed to operate as both a Bluetooth Low Energy (BLE) peripheral and a wired USB HID controller. The system combines a high-resolution magnetic encoder with a symmetric button layout and display-driven mode feedback.

The project focuses on system-level design tradeoffs across RF, power management, embedded firmware, and manufacturable PCB design.

---

## Key Features

- Dual-mode operation: BLE + USB HID
- Magnetic encoder-based primary input (AS5600L)
- Symmetric 8-button layout supporting left/right-handed use
- OLED display for mode and mapping feedback
- LiPo-powered system with onboard charging and protection
- Low-power architecture with wake-on-interaction
- Planned modular expansion (secondary wheel interface)

---

## System Architecture

### Core Components

- MCU: nRF52840 (USB + BLE capable package)
- Power:
  - Single-cell LiPo battery
  - Charger + protection + load switching
- Inputs:
  - Magnetic encoder (AS5600L)
  - 8 mechanical switches
- Output:
  - BLE HID (primary wireless interface)
  - USB HID (wired fallback)
- Display:
  - SPI OLED

### Architecture Overview
[Battery] → [Charger / Protection] → [Power Switching] → [nRF52840]
│
├── BLE (Wireless HID)
├── USB-C (Wired HID)
├── Encoder (I2C)
├── Button Matrix (GPIO)
└── OLED (SPI)


---

## Key Design Decisions

### Package Selection

Initial design targeted the nRF52840 QFN48 package to simplify assembly. This variant does not support the USBD peripheral, which conflicts with the requirement for wired USB HID. The design was revised to use a USB-capable package.

### USB vs BLE

BLE provides flexibility and wireless operation but introduces pairing and reconnection complexity. USB HID is retained as a deterministic fallback for reliability and compatibility.

### RF Design Approach

The RF front-end follows Nordic’s reference design for impedance matching and antenna layout to minimize risk in the initial revision.

### Assembly Strategy

Rev A is designed for hand assembly using solder paste and hot air reflow. Component sizes are standardized to 0402 where possible, with 0201 components limited to RF-critical sections.

---

## Current Status

- [x] Concept definition
- [x] Component selection
- [x] Package trade study (QFN48 vs USB-capable variant)
- [ ] Schematic (in progress)
- [ ] PCB layout
- [ ] Hardware bring-up
- [ ] Firmware development

---

## Repository Structure
/hardware
/schematics
/pcb
/manufacturing

/firmware

/docs

/logs


---

## Development Notes

Detailed design decisions, trade studies, and debugging notes are maintained in the `/logs` directory.