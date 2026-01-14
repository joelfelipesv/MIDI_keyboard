# MIDI_keyboard

A high-performance hardware-software interface designed for digital music production. This controller bridges physical inputs with Digital Audio Workstations (DAWs) through a serial-to-MIDI bridge, featuring real-time visual feedback on an integrated IPS display.

## 🚀 Overview

This MIDI controller was built using the AtMega2560 micro-architecture to handle high-density digital and analog inputs with sub-10ms latency. Encased in a custom acrylic shell, it provides a tactile and visual experience for professional music production in software like Ableton Live, FL Studio, or Reaper.

## 🛠️ Technical Specifications

Microcontroller: AtMega2560 (8-bit AVR).

Physical Controls:

- 12 Digital Buttons: Mapped to MIDI Note On/Note Off messages.

- 4 Analog Knobs: Mapped to Control Change (CC) messages (range 0-127).

- Visual Feedback: Integrated IPS ST7789 Display (240x240 px) providing real-time musical notation (e.g., "C4") using high-speed SPI communication.

- Connectivity: USB Type B (Serial-over-USB).

- Enclosure: Custom-built closed acrylic housing.

## 💻 System Architecture

To ensure compatibility with standard MIDI drivers on Windows, the system uses a serial-to-virtual-MIDI bridge:

Firmware: Written in C++ (Arduino) to process input matrix and drive the ST7789 display.

LoopMIDI: Creates a virtual MIDI port on the OS.

Hairless MIDI Bridge: Translates raw serial data from the microcontroller into standard MIDI messages.

## ⚙️ Setup & Installation

Create Virtual Port: Install loopMIDI and create a port named MIDI-Controller.

Start Bridge: Open Hairless MIDI, select your controller's COM port, and set the MIDI Out to the virtual port created in step 1.

DAW Configuration: In your preferred DAW, enable the MIDI-Controller port as an active MIDI Input device.

## 📈 Engineering Insights & Learnings

Low-Latency Loop Optimization: Optimized the display refresh rate to avoid blocking the MIDI signal processing loop.

Hardware Debouncing: Implemented software-side state management for the 12-button matrix to eliminate ghost triggers.

Data Integrity: Managing high-speed serial communication (Baud rate optimization) to prevent MIDI message jitter.

## 📫 ##Contact

Joel Felipe Suárez Vidarte LinkedIn | Email

Developed as part of an exploration into Human-Computer Interaction (HCI) and Embedded Systems.
