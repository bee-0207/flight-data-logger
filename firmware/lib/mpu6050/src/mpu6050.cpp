#include "mpu6050.h"

namespace {
constexpr uint8_t REG_SMPLRT_DIV   = 0x19;
constexpr uint8_t REG_CONFIG       = 0x1A;
constexpr uint8_t REG_GYRO_CONFIG  = 0x1B;
constexpr uint8_t REG_ACCEL_CONFIG = 0x1C;
constexpr uint8_t REG_ACCEL_XOUT_H = 0x3B;
constexpr uint8_t REG_PWR_MGMT_1   = 0x6B;
constexpr uint8_t REG_WHO_AM_I     = 0x75;

constexpr uint8_t WHO_AM_I_VALUE   = 0x68;
constexpr float   GRAVITY          = 9.80665f;

int16_t be16(const uint8_t *p) {
    return static_cast<int16_t>((p[0] << 8) | p[1]);
}
}  // namespace

Mpu6050::Mpu6050(TwoWire &wire, uint8_t address) : wire_(wire), address_(address) {}

bool Mpu6050::begin(AccelRange accel, GyroRange gyro) {
    uint8_t id = 0;
    if (!readRegs(REG_WHO_AM_I, &id, 1) || id != WHO_AM_I_VALUE) {
        return false;
    }

    // Wake up, use the X-gyro PLL as clock source (more stable than the internal oscillator).
    if (!writeReg(REG_PWR_MGMT_1, 0x01)) return false;
    delay(10);

    // Digital low-pass filter ~44 Hz; sample rate = 1 kHz / (1 + 0) = 1 kHz.
    if (!writeReg(REG_CONFIG, 0x03)) return false;
    if (!writeReg(REG_SMPLRT_DIV, 0x00)) return false;

    if (!writeReg(REG_ACCEL_CONFIG, static_cast<uint8_t>(accel) << 3)) return false;
    if (!writeReg(REG_GYRO_CONFIG, static_cast<uint8_t>(gyro) << 3)) return false;

    accel_lsb_per_g_  = 16384.0f / (1 << static_cast<uint8_t>(accel));
    gyro_lsb_per_dps_ = 131.0f   / (1 << static_cast<uint8_t>(gyro));
    return true;
}

bool Mpu6050::read(ImuSample &out) {
    // ACCEL_XOUT_H .. GYRO_ZOUT_L: 14 bytes (accel xyz, temp, gyro xyz), big-endian.
    uint8_t buf[14];
    if (!readRegs(REG_ACCEL_XOUT_H, buf, sizeof(buf))) return false;

    const float a_scale = GRAVITY / accel_lsb_per_g_;
    out.ax = be16(&buf[0]) * a_scale;
    out.ay = be16(&buf[2]) * a_scale;
    out.az = be16(&buf[4]) * a_scale;
    out.temp_c = be16(&buf[6]) / 340.0f + 36.53f;
    out.gx = be16(&buf[8])  / gyro_lsb_per_dps_;
    out.gy = be16(&buf[10]) / gyro_lsb_per_dps_;
    out.gz = be16(&buf[12]) / gyro_lsb_per_dps_;
    return true;
}

bool Mpu6050::writeReg(uint8_t reg, uint8_t value) {
    wire_.beginTransmission(address_);
    wire_.write(reg);
    wire_.write(value);
    return wire_.endTransmission() == 0;
}

bool Mpu6050::readRegs(uint8_t reg, uint8_t *buf, size_t len) {
    wire_.beginTransmission(address_);
    wire_.write(reg);
    if (wire_.endTransmission(false) != 0) return false;  // repeated start

    if (wire_.requestFrom(address_, static_cast<uint8_t>(len)) != len) return false;
    for (size_t i = 0; i < len; ++i) {
        buf[i] = wire_.read();
    }
    return true;
}
