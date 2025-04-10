#include <Arduino.h>
#include <ESPDateTime.h>
#include <cmath>
#include <sensor.h>
#include <display.h>
#include <backend.h>

Display display;
Sensor sensor;
Backend backend;

char *SSID = const_cast<char *>("AndroidAP463F");                        // TODO replace with own wifi credentials
char *PW = const_cast<char *>("trabzon61");                      // TODO replace with own wifi credentials
const String BACKEND_UUID = "7c5e672e-cbf3-11ec-9d64-0242ac120002"; // TODO replace with credentials provided by email
const String BACKEND_TOKEN = "wl0qpvcz4s";                          // TODO replace with credentials provided by email
const float SENSOR_LAT = 48.83876;                                 // TODO replace with own coordinates (example: HHN)
const float SENSOR_LON = 9.19225;                                  // TODO replace with own coordinates
const bool SENSOR_INDOOR = true;

// Hinweise: Erst für Temperatur, später dann auch für Druck (analoges Vorgehen)

// Schritt 1-3: in Sensor::initSensor()

// 1. Schritt
// BMP280 finden
// -> read8 implementieren

// 2. Schritt
// Korrekturwerte der Temperatur (trimming values) lesen
// -> read16 implementieren

// 3. Schritt
// Sensor initialisieren (Modus allgemein, Modi für Temperatur und Druck)

// 4. Schritt: Korrekturwerte (zum Testen) auf der Konsole ausgeben lassen

// Schritt 5-6: in Sensor::readTemperatureAndPressure()

// 5. Schritt: Lesen der Temperaturwerte (raw)

// 6. Schritt: Konvertieren der "raw" Temperaturwerte in °C und Speichern in "temperature"

// 7. Schritt in Display::refreshContent() implementieren

// 8. Schritt: 2-7 analog für Luftdruck

void setup()
{

  Serial.begin(9600);

  display.init();

  backend.init(BACKEND_UUID, BACKEND_TOKEN, SENSOR_LAT, SENSOR_LON, SENSOR_INDOOR);

  backend.connectToWifi(SSID, PW);
  display.setSsid(SSID);

  if (!DateTime.begin(15 * 1000))
  {
    Serial.println("Read current time failed!");
  }
  sensor.initSensor();
}

void loop()
{

  bool readSuccessful = sensor.readTemperatureAndPressure();
  display.setReadSuccessful(readSuccessful);

  if (readSuccessful)
  {
    float t = sensor.getTemperature();
    float p = sensor.getPressure();
    display.setTemperature(t);
    display.setPressure(p);
    backend.sendTemperatureMeasurement(t, DateFormatter::format("%Y-%m-%dT%H:%M:%S.000Z", DateTime.now()));
    backend.sendPressureMeasurement(p, DateFormatter::format("%Y-%m-%dT%H:%M:%S.000Z", DateTime.now()));
  }

  delay(3000);
}