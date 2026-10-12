#include <Arduino.h>
#include <Wire.h>

#include "mpu6050.h"
#include "pins.h"

// First milestone: read the IMU at a fixed rate and stream CSV over serial.
// Open the serial monitor at 115200 baud (`pio device monitor`).

constexpr uint32_t SAMPLE_PERIOD_US = 10000;  // 100 Hz

Mpu6050 imu;

void setup() {
    Serial.begin(115200);
    pinMode(PIN_LED, OUTPUT);

    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL, 400000);  // 400 kHz fast-mode I2C

    while (!imu.begin()) {
        Serial.println("MPU6050 not found -- check wiring (SDA=21, SCL=22, VCC=3.3V)");
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        delay(500);
    }
    digitalWrite(PIN_LED, HIGH);

    Serial.println("t_us,ax,ay,az,gx,gy,gz,temp_c");
}

void loop() {
    static uint32_t next_us = micros();

    // Fixed-rate loop: wait until the next sample slot.
    if (static_cast<int32_t>(micros() - next_us) < 0) return;
    next_us += SAMPLE_PERIOD_US;

    ImuSample s;
    if (!imu.read(s)) {
        Serial.println("# IMU read error");
        return;
    }

    Serial.printf("%lu,%.3f,%.3f,%.3f,%.2f,%.2f,%.2f,%.1f\n",
                  static_cast<unsigned long>(micros()),
                  s.ax, s.ay, s.az, s.gx, s.gy, s.gz, s.temp_c);
}
