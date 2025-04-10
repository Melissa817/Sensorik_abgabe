#include <Arduino.h>
#include <Wire.h>
#include <limits>
#include <sensor.h>
#include <math.h>
#include <cmath>

#define I2C_SDA 21
#define I2C_SCL 13
#define I2C_ADDR 0x76

/* uint16_t Sensor::readDummy(byte reg) {
  if successful
    return value;
  if error
    throw errorcode;
} */

/**
 * @brief 
 * @param reg 
 * @return uint16_t 
 * 0x76. Es startet eine Übertragung.
 * Ohne dem Request ließt der Sensor die gemessenen Werte nicht aus, 
 * bei der Temperatur kommt 0° und Pressure nan raus.
 * i2c.requestFrom: die Bytes werden angefordert. I2C_ADDR = Adresse, 2 = Anzahl der geforderten Bytes(16 Bits).
 */
uint16_t Sensor::read16(byte reg)
{
  uint16_t u16;
  i2c.beginTransmission(I2C_ADDR);  
  i2c.write(reg);
  i2c.endTransmission();
  i2c.requestFrom(I2C_ADDR,2);
  
  return u16 = i2c.read() + (i2c.read() * 256);
}

/**
 * @brief 
 * @param reg 
 * @return uint8_t 
 * 0x76. Es startet eine Übertragung.
 * Ohne dem Request ließt der Sensor die gemessenen Werte nicht aus, 
 * bei der Temperatur kommt 0° und Pressure nan raus.
 * i2c.requestFrom: die Bytes werden angefordert. I2C_ADDR = Adresse, 1 = Anzahl der geforderten Bytes(8 Bits).
 */
uint8_t Sensor::read8(byte reg)
{
  uint8_t u8;
  i2c.beginTransmission(I2C_ADDR); // 0x76
  i2c.write(reg);
  i2c.endTransmission();
  i2c.requestFrom(I2C_ADDR,1);  //ohne dem Request ließt der Sensor die gemessenen Werte nicht aus. Bei der Temperatur kommt 0° und Pressure nan raus.
  u8 = i2c.read();

  return u8;
}

   

/**
 * @brief 
 * I2C Starten mit SDA, SCL
 * Temp-Sensor suchen (BMP280)
 * Korrekturwerte (trimming values) lesen und in dig_T1 - T3... dig_P1 - P9 speichern mit den entsprechenden Datentypen.
 */
void Sensor::initSensor()
{
   try{
   // I2C Starten mit SDA, SCL
   i2c.setPins(I2C_SDA,I2C_SCL);
    if(i2c.begin() == true){
    if(read8(0xD0)==0x58){

        i2c.beginTransmission(I2C_ADDR);
        i2c.write(0xF4);
        i2c.write(0b11110111);
        i2c.endTransmission();

        dig_P1=(unsigned)read16(0x8E);
        dig_P2=(short)read16(0x90);
        dig_P3=(short)read16(0x92);
        dig_P4=(short)read16(0x94);
        dig_P5=(short)read16(0x96);
        dig_P6=(short)read16(0x98);
        dig_P7=(short)read16(0x9A);
        dig_P8=(short)read16(0x9C);
        dig_P9=(short)read16(0x9E);
        dig_T1=(unsigned)read16(0x88);
        dig_T2=(short)read16(0x8A);
        dig_T3=(short)read16(0x8C);
    }
  }
   }
  catch (uint16_t err)
  {
    Serial.println("Sensor error: " + String(err));
  }
}

/**
 * @brief 
 * @return float temperature
 */
float Sensor::getTemperature()
{
  return temperature;
}

/**
 * @brief 
 * @return float pressure
 */
float Sensor::getPressure()
{
  return pressure;
}

/**
 * @brief 
 * @return true 
 * @return false 
 * return temperature und pressure mit den gegebenen Formeln/Gleichung aus der Vorlesung.
 */
bool Sensor::readTemperatureAndPressure()
{
  try
  {
    // read temperature
    uint8_t w1Temperature=read8(0xFA);   // most significant
    uint8_t w2Temperature=read8(0xFB);
    uint8_t w3Temperature=read8(0xFC) >> 4;  // least significant
    
    signed long int wertT = w1Temperature*pow(2,12) + w2Temperature*pow(2,4) + w3Temperature; //gesamt 20 bit
    
    uint8_t data = read8(dig_T1);
    if(dig_T2 > 32767.0){
      dig_T2 -= 65536;
    }
    dig_T3 = data * 256 + data;

    if(dig_T3 > 32767.0){
      dig_T3 -= 65536;
    }

    float adctT = float(wertT);
    float var1T = (adctT/16384.0 - dig_T1/1024.0) * dig_T2;
    float var2T = (adctT/131072.0 - dig_T1/8192.0);
    float tFineT = var1T + var2T;
    float temp = tFineT / 5120.0; 

    temperature = temp;

    // read pressure

    uint8_t w1Pressure=read8(0xF7);
    uint8_t w2Pressure=read8(0xF8);
    uint8_t w3Pressur=read8(0xF9) >> 4;
    
    signed long int wertP = w1Pressure*pow(2,12) + w2Pressure*pow(2,4) + w3Pressur;
    float tFineP = temp * 5120.0;
    float adcP = wertP;

    float var1P = tFineP/2.0 -64000.0;
    float var2P = var1P * var1P * dig_P6/32768.0;
    var2P = var2P + var1P * dig_P5 * 2.0;
    var2P = var2P/4.0 + dig_P4 * 65536.0;

    var1P = (dig_P3 * var1P * var1P/524288.0 + dig_P2 * var1P) / 524288.0;
    var1P = (1.0 + var1P/32768.0) * dig_P1;
    float p = 1048576.0 - adcP;
    p = (p - var2P/4096.0) * 6250.0/var1P;
    var1P = dig_P9 * p * p/2147483648.0;
    var2P = p * dig_P8/32768.0;
    p = p + (var1P + var2P + dig_P7)/16.0;

    p=p/100.0;

    // pressure mit der Sesnorkalibrierungswerte korrigiert, somit erhält man hohe und genaue Luftdruckwerte.
    float altitude = 252.0; // die Meereshöhe meines Standorts (Stuttgart-Zazenhausen)
    float pLocal = p * 100;
    float pNN = pLocal/pow(1-altitude/44330.0, 5.255);

    pressure = pNN/100;
    
    // del end
    return true;
  }
  catch (uint8_t err)
  {
    Serial.println("Sensor error: " + String(err));
    return false;
  }
}