#pragma once

// Pin assignments for a standard ESP32-WROOM DevKit.
// Change these to match your wiring.

// I2C bus (IMU + barometer)
#define PIN_I2C_SDA 21
#define PIN_I2C_SCL 22

// SPI bus (microSD) -- VSPI defaults
#define PIN_SD_SCK  18
#define PIN_SD_MISO 19
#define PIN_SD_MOSI 23
#define PIN_SD_CS    5

// Status LED (on-board LED on most DevKits)
#define PIN_LED      2
