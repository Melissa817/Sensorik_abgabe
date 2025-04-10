#ifndef SENSOR_H_INCLUDED
#define SENSOR_H_INCLUDED

#include <Wire.h>

class Sensor
{
public:
    void initSensor();

    bool readTemperatureAndPressure();

    /**
     * Returns the last read temperature in degrees celcius. Returns std::numeric_limits<float>::min() if the temperature is not available.
     */
    float getTemperature();
    /**
     * Returns the last read pressure in hPa. Returns std::numeric_limits<float>::min() if the pressure is not available.
     */
    float getPressure();

private:
    TwoWire i2c = TwoWire(1);

    // trimming values
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;

    // last values
    float temperature = std::numeric_limits<float>::min();
    float pressure = std::numeric_limits<float>::min();

    /**
     * read 16 bit (2 bytes) from BMP280, starting at register reg
    */
    uint16_t read16(byte reg);

    /**
     * read 8 bit (1 byte) from BMP280, starting at register reg
    */
    uint8_t read8(byte reg);
};

#endif
