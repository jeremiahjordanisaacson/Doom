# DOOM - ESP32 Microcontroller Port (Proof of Concept)

**Status: Proof of Concept**

## Description

ESP32 microcontroller proof-of-concept port of PrBoom. Renders to an SPI-connected display (ILI9341 or similar TFT panel) using the ESP-IDF framework. The ESP32's WiFi capability opens the door for potential network multiplayer over WiFi.

## Target Hardware / OS

- ESP32 (dual-core Xtensa LX6, 240 MHz)
- ESP32-S3 (recommended, with PSRAM)
- SPI display: ILI9341 (320x240), ST7789 (240x240), or similar
- 4MB+ Flash, 8MB PSRAM recommended
- FreeRTOS (via ESP-IDF)

## Build Toolchain

- ESP-IDF v5.0+ (Espressif IoT Development Framework)
- Xtensa GCC cross-compiler (included with ESP-IDF)
- CMake 3.16+ (included with ESP-IDF)
- Python 3.8+ (ESP-IDF build system dependency)
- esptool.py (for flashing)

## Build Instructions

```bash
# Install ESP-IDF
# See https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/

# Source ESP-IDF environment
. $IDF_PATH/export.sh

# Configure
idf.py set-target esp32s3
idf.py menuconfig  # Configure display pins, PSRAM, etc.

# Build
idf.py build

# Flash to device
idf.py -p /dev/ttyUSB0 flash

# Monitor serial output
idf.py -p /dev/ttyUSB0 monitor
```

> **Note:** Build instructions are preliminary and subject to change as the port is developed.

## Platform-Specific Notes

- The ESP32 has 520KB internal SRAM; PSRAM (up to 8MB) is strongly recommended for WAD data.
- Rendering is done in software and pushed to the SPI display via DMA for minimal CPU overhead.
- Display resolution is typically 320x240 at 20-30fps depending on scene complexity.
- Input can be via GPIO buttons, I2C gamepad, or Bluetooth gamepad (ESP32 has built-in Bluetooth).
- WAD file is stored in SPIFFS/LittleFS partition on flash, or loaded from an SD card.
- WiFi support enables potential LAN multiplayer and OTA firmware updates.
- Dual-core ESP32 can dedicate one core to rendering and one core to game logic.
- Power consumption is approximately 150-250mA; can be battery powered for portable builds.
- This port demonstrates that DOOM can run on modern microcontrollers costing under $5.

## License

This project is licensed under the **GNU General Public License v2.0** (GPL v2).
See the [LICENSE](../../LICENSE) file in the repository root for details.
