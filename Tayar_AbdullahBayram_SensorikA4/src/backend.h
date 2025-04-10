#ifndef BACKEND_H_INCLUDED
#define BACKEND_H_INCLUDED

#include <Arduino.h>

class Backend
{
public:
    void init(String uuid, String token, float latitude, float longitude, bool indoor);
    void connectToWifi(char *ssid, char *password);
    void sendMeasurement(String type, float value, String timestamp);
    void sendTemperatureMeasurement(float value, String timestamp);
    void sendPressureMeasurement(float value, String timestamp);

private:
    String uuid;
    String token;
    float latitude;
    float longitude;
    bool indoor;
};

#endif