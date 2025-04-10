#include <backend.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

void Backend::connectToWifi(char *ssid, char *password)
{
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());
}

void Backend::init(String uuid, String token, float latitude, float longitude, bool indoor)
{
  this->uuid = uuid;
  this->token = token;
  this->latitude = latitude;
  this->longitude = longitude;
  this->indoor = indoor;
}

void Backend::sendMeasurement(String type, float value, String timestamp)
{
  if ((WiFi.status() == WL_CONNECTED))
  {
    HTTPClient client;
    client.begin("https://sensorik.it.hs-heilbronn.de/api/mqC");
    client.addHeader("Content-Type", "application/json");

    String input = "{\"device\": {\"token\": \"" + token + "\",\"uuid\": \"" + uuid + "\"},\"measurementType\": \"" + type + "\",\"timestamp\": \"" + timestamp + "\",\"value\": " + value + ",\"latitude\": " + latitude + ",\"longitude\": " + longitude + ",\"indoor\": " + indoor + "}";

    int httpCode = client.POST(input);

    if (httpCode > 0)
    {
      String payload = client.getString();
      Serial.println("\nStatuscode:" + String(httpCode));
      Serial.println(payload);
    }
    else
    {
      Serial.println("Connection lost");
    }
  }
  else
  {
    Serial.println("Connection lost");
  }
}

void Backend::sendTemperatureMeasurement(float value, String timestamp)
{
  sendMeasurement("Temp", value, timestamp);
}

void Backend::sendPressureMeasurement(float value, String timestamp)
{
  sendMeasurement("Pressure", value, timestamp);
}