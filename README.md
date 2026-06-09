ST25R100 RFID/NFC Firmware Driver

This repository contains two independent approaches for interfacing with the ST25R100:

1. Custom ISO14443-A / NFC-A Implementation

A lightweight RFID stack developed from the ground up specifically for the NFC-A protocol. The implementation directly manages ST25R100 register configuration, RF field control, frame transmission/reception without relying on vendor middleware.

2. RFAL Port to PSoC 6

Port of STMicroelectronics' RFAL (RF Abstraction Layer) from its native STM32 environment to the PSoC 6 platform. This effort involved adapting hardware abstraction layers, timing functions, SPI interfaces, interrupts, and platform-specific dependencies while preserving RFAL functionality.


Technical Highlights:
Embedded C development
SPI peripheral driver development
RFID/NFC protocol implementation
Hardware abstraction layer (HAL) design
Cross-platform middleware porting
Real-time timing and interrupt handling
Low-level RF transceiver control

This project explores both ends of the embedded firmware spectrum: developing a protocol stack from scratch, and porting a production-grade vendor middleware stack to a new hardware platform.
