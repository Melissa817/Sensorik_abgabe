#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED


#include <Arduino.h>
#include <SSD1306.h>

class Display {
    public:
        void init();
        void setSsid(char* ssid);
        void setReadSuccessful(bool successful);
        void setTemperature(float temperature);
        void setPressure(float pressure);

    private:
        SSD1306 display = SSD1306(0x3C, 4, 15);
        bool lastReadSuccessful = false;
        float lastTemperature = 999.99;
        float lastPressure = 999.99;
        String wifiSsid = "";
        void refreshContent();
};


#endif