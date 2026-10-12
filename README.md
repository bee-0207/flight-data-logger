# Flight Data Logger

A small flight computer that reads an IMU and barometer, fuses the data to estimate
orientation and altitude, and logs everything to microSD for post-flight analysis.
Built to fly in a model rocket or on a drone.

> 🚧 **Status:** early development

## Features (planned)

- [ ] Read IMU (accelerometer + gyroscope) over I²C/SPI
- [ ] Read barometric pressure / temperature
- [ ] Complementary filter for orientation (roll / pitch)
- [ ] Kalman filter for altitude and vertical velocity
- [ ] Timestamped logging to microSD (CSV or binary)
- [ ] Flight state detection (idle → boost → coast → apogee → descent → landed)
- [ ] Python tools to parse logs, plot data, and render a 3D flight replay

## Hardware

| Part | Example | Purpose |
|------|---------|---------|
| Microcontroller | STM32 (e.g. Nucleo / Black Pill) or ESP32 | Main processor |
| IMU | MPU6050 or BMI270 | Acceleration + angular rate |
| Barometer | BMP390 | Pressure → altitude |
| Storage | microSD breakout | Flight logs |
| Power | 1S LiPo + regulator | Battery power |

Wiring diagram and schematics: see [`hardware/`](hardware/).

## Repository layout

```
firmware/   Microcontroller code (src/, include/, lib/)
tools/      Python scripts for parsing, plotting, and 3D visualization
data/       Sample flight logs
hardware/   Schematics, wiring diagrams, BOM, PCB files
docs/       Design notes, images, and test results
```

## Getting started

_Build and flash instructions coming once the toolchain is chosen._

## Results

_Flight plots and test data will go here._

## Authors

- [@bee-0207](https://github.com/bee-0207)
- _Collaborator — add yourself here!_

## License

[MIT](LICENSE)
