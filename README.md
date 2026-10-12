# Flight Data Logger

A small flight computer that reads an IMU and barometer, fuses the data to estimate
orientation and altitude, and logs everything to microSD for post-flight analysis.
Built to fly in a model rocket or on a drone.

> 🚧 **Status:** early development

[![Firmware build](https://github.com/bee-0207/flight-data-logger/actions/workflows/firmware.yml/badge.svg)](https://github.com/bee-0207/flight-data-logger/actions/workflows/firmware.yml)

## Features (planned)

- [x] Read IMU (accelerometer + gyroscope) over I²C — custom MPU6050 driver
- [ ] Read barometric pressure / temperature
- [ ] Complementary filter for orientation (roll / pitch)
- [ ] Kalman filter for altitude and vertical velocity
- [ ] Timestamped logging to microSD (CSV or binary)
- [ ] Flight state detection (idle → boost → coast → apogee → descent → landed)
- [ ] Python tools to parse logs, plot data, and render a 3D flight replay

## Hardware

| Part | Example | Purpose |
|------|---------|---------|
| Microcontroller | ESP32-WROOM DevKit (esp32dev) | Main processor |
| IMU | MPU6050 or BMI270 | Acceleration + angular rate |
| Barometer | BMP390 | Pressure → altitude |
| Storage | microSD breakout | Flight logs |
| Power | 1S LiPo + regulator | Battery power |

### Wiring

| Signal | ESP32 pin | Connects to |
|--------|-----------|-------------|
| I²C SDA | GPIO 21 | MPU6050 SDA, BMP390 SDA |
| I²C SCL | GPIO 22 | MPU6050 SCL, BMP390 SCL |
| SPI SCK | GPIO 18 | microSD SCK |
| SPI MISO | GPIO 19 | microSD MISO |
| SPI MOSI | GPIO 23 | microSD MOSI |
| SD CS | GPIO 5 | microSD CS |
| 3.3 V / GND | 3V3 / GND | All sensors |

Pin definitions live in [`firmware/include/pins.h`](firmware/include/pins.h).
Schematics: see [`hardware/`](hardware/).

## Repository layout

```
firmware/   Microcontroller code (src/, include/, lib/)
tools/      Python scripts for parsing, plotting, and 3D visualization
data/       Sample flight logs
hardware/   Schematics, wiring diagrams, BOM, PCB files
docs/       Design notes, images, and test results
```

## Getting started

Firmware uses [PlatformIO](https://platformio.org/) with the Arduino framework for ESP32.

1. Install [VS Code](https://code.visualstudio.com/) and the **PlatformIO IDE** extension
   (or the CLI: `pip install platformio`).
2. Open the `firmware/` folder in VS Code.
3. Plug in the ESP32 and click **Upload** (→) in the PlatformIO toolbar, or run:

   ```sh
   cd firmware
   pio run -t upload      # build + flash
   pio device monitor     # view serial output at 115200 baud
   ```

You should see CSV lines of IMU data at 100 Hz. Lay the board flat: `az` should read about 9.8 m/s².

Every push that touches `firmware/` is compiled by GitHub Actions.

## Results

_Flight plots and test data will go here._

## Authors

- [@bee-0207](https://github.com/bee-0207)
- _Collaborator — add yourself here!_

## License

[MIT](LICENSE)
