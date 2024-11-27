#pragma once

class TempSensor
{
    private:
        long lastmillis = millis();

    public:
        TempSensor();

        int16_t getTemp();
       
};