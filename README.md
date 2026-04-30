# DAQ-Board-Firmware

Firmware for the G08 Capstone DAQ built on the STM32 NUCLEO-F767ZI platform.

## Overview

This repository contains the embedded firmware used to acquire ADC data, move it through the MCU over SPI, and stream it to a host PC over USB CDC.

The current firmware includes:

- SPI1 communication with the ADS131M04 ADC
- DMA-based buffering of ADC frames
- FreeRTOS tasks for USB receive and transmit handling
- USB CDC streaming for captured data

## Repository Layout

- `DAQ_Nucleo_F767ZI_Firmware/Core` contains the CubeMX-generated application code and RTOS tasks.
- `DAQ_Nucleo_F767ZI_Firmware/Drivers` contains the ADC peripheral driver and STM32 HAL sources.
- `DAQ_Nucleo_F767ZI_Firmware/USB_DEVICE` contains the USB CDC device implementation.
- `TestingSoftware` contains host-side scripts used for validation and data capture testing.

## Firmware Flow

1. The MCU initializes the clock, peripherals, DMA, SPI1, CRC, and USB.
2. The ADS131M04 is reset and configured.
3. ADC samples are collected through SPI DMA into the receive buffer.
4. Buffered samples are repackaged and transmitted to the host over USB CDC.

## Building and Flashing

The project is structured as an STM32CubeIDE firmware workspace. Open the `DAQ_Nucleo_F767ZI_Firmware` project in STM32CubeIDE, build the project, and flash it to the NUCLEO-F767ZI board.

## Known Issues

The system cannot currently run at the full desired sampling rate because the SPI communication over Du Pont style wires is slower than the required data rate. Fixing this requires a hardware redesign so the ADC link can sustain the target throughput.

Once the hardware bottleneck is removed, the ADC configuration code can be updated to increase the sampling rate.

The circular buffer and DMA receive path may also need additional testing to verify that data is captured and forwarded without loss or corruption.

## Next Steps

- Redesign the ADC wiring and signal path for higher-speed operation.
- Increase the ADC sampling configuration after the hardware path can support it.
- Validate the circular buffer behavior under sustained acquisition.
