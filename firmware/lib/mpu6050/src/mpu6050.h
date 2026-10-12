#pragma once

#include <Arduino.h>
#include <Wire.h>

// Minimal register-level driver for the InvenSense MPU-6050 IMU.
// Datasheet / register map: https://invensense.tdk.com/products/motion-tracking/6-axis/mpu-6050/

struct ImuSample {
    float ax, ay, az;  // acceleration [m/s^2]
    float gx, gy, gz;  // angular rate [deg/s]
    float temp_c;      // die temperature [C]
};

class Mpu6050 {
public:
    enum class AccelRange : uint8_t { G2 = 0, G4 = 1, G8 = 2, G16 = 3 };
    enum class GyroRange  : uint8_t { DPS250 = 0, DPS500 = 1, DPS1000 = 2, DPS2000 = 3 };

    explicit Mpu6050(TwoWire &wire = Wire, uint8_t address = 0x68);

    // Wakes the sensor and applies the ranges. Returns false if the chip
    // doesn't answer or WHO_AM_I doesn't match.
    bool begin(AccelRange accel = AccelRange::G16, GyroRange gyro = GyroRange::DPS2000);

    // Reads all axes in one burst. Returns false on an I2C error.
    bool read(ImuSample &out);

private:
    bool writeReg(uint8_t reg, uint8_t value);
    bool readRegs(uint8_t reg, uint8_t *buf, size_t len);

    TwoWire &wire_;
    uint8_t address_;
    float accel_lsb_per_g_   = 2048.0f;
    float gyro_lsb_per_dps_  = 16.4f;
};
