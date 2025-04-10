#include <display.h>

void Display::refreshContent()
{
  display.clear();
  display.drawString(8, 5, "Temperature: " + String(lastTemperature) + " C°"); // Datum hatte ich auch drin, aber es hat eine falsche Uhrzeit angezeigt(2h zurück).
  display.drawString(8, 45, "Pressure: " + String(lastPressure) + " P");
  display.display();

  // Beispielaufbau
  // 1. Zeile AI-Sensorik
  // 2. Zeile: Temperatur oder Fehlermeldung
  // 3. Zeile: Mit WiFi verbunden? + Name

  // Hinweis: nicht vergessen, den Platzhalter oben zu entfernen!
}

void Display::init()
{
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  delay(50);
  digitalWrite(16, HIGH);
  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  //display.setColor(WHITE);
  

  refreshContent();

  delay(400);
}

void Display::setReadSuccessful(bool successful)
{
  lastReadSuccessful = successful;
  refreshContent();
}

void Display::setSsid(char *ssid)
{
  wifiSsid = String(ssid);
  refreshContent();
}

void Display::setTemperature(float temperature)
{
  lastTemperature = temperature;
  refreshContent();
}

void Display::setPressure(float pressure)
{
  lastPressure = pressure;
  refreshContent();
}