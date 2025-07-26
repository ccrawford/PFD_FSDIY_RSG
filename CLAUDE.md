# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an Arduino-based firmware project for a flight simulator G1000 control board hardware interface. The project creates a hardware controller that communicates with RealSimGear G1000 software via serial protocol, handling various aviation instrument controls including encoders, buttons, and soft keys.

## Build System

The project uses PlatformIO as the build system:

### Development Commands
- `pio run` - Build the project
- `pio run --target upload` - Build and upload to Arduino Mega 2560
- `pio run --target monitor` - Open serial monitor
- `pio run --target clean` - Clean build artifacts

### Hardware Target
- **Board**: Arduino Mega 2560 (ATmega2560)
- **Platform**: Atmel AVR
- **Framework**: Arduino
- **Baud Rate**: 115200

## Architecture

### Core Components

1. **Button System** (`src/Button.h`, `src/Button.cpp`)
   - Custom debounced button library
   - Handles press/release detection with configurable debounce timing
   - Auto-repeat functionality for continuous button presses
   - Serial output format: `BUTTON_NAME=1` (pressed) or `BUTTON_NAME=0` (released)

2. **Encoder Handling** (`src/main.cpp:19-47`)
   - `RotaryHandler` abstract base class
   - `QuadPulseRotaryHandler` implementation for quadrature encoders
   - Configurable pulse count per detent (2 or 4 pulses typical)
   - Serial output format: `ENC_NAME_UP` or `ENC_NAME_DN`

3. **Multiplexer Integration**
   - Uses analog-digital multiplexer library for expanding I/O
   - Handles soft keys through multiplexed inputs
   - Pin configuration: SIG=33, S0-S3=41,39,37,35

### Hardware Configuration

The firmware is configured for FSDIY G1000 Control Board v2.3 with:
- **29 direct buttons** connected to various pins
- **14 rotary encoders** for flight controls (NAV, COM, ALT, HDG, etc.)
- **16 soft keys** via multiplexer
- **RealSimGear protocol** keep-alive mechanism

### Serial Protocol

The device communicates using a custom RealSimGear protocol:
- Keep-alive: `\####RealSimGear#RealSimGear-G1000XFD1#1#3.1.9#756E6B776F06E/` (every 3 seconds)
- Heartbeat: newline character (every 100ms)
- Button events: `BUTTON_NAME=1` or `BUTTON_NAME=0`
- Encoder events: `ENC_NAME_UP` or `ENC_NAME_DN`

## Development Notes

### Pin Assignments
- Encoders use specific pin pairs for A/B channels (see `src/main.cpp:60-73`)
- Direct buttons are assigned individual pins (see `src/main.cpp:104-136`)
- Multiplexer uses pins 33, 35, 37, 39, 41 for signal and select lines

### Libraries Used
- **Encoder** (v1.4.4): Quadrature encoder reading with interrupt optimization
- **Analog-Digital Multiplexers** (v3.0.0): I/O expansion for soft keys
- Custom **Button** library: Local debounced button handling

### Code Patterns
- All hardware initialization occurs in `setup()`
- Main loop processes encoders first (with short-circuit for responsiveness)
- Button and multiplexer scanning follows encoder processing
- Serial output uses consistent naming conventions for flight sim integration

### Key Constants
- `NUM_BUTTONS`: 29 total direct buttons
- `NUM_ENCODERS`: 14 rotary encoders
- Encoder pulse counts: 2 for fine controls, 4 for volume/coarse controls
- Debounce timing: 10ms default for buttons