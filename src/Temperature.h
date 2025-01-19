#pragma once

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "Definitions.h"

class TempSensor
{
    private:
        long lastmillis = millis();

    public:
        TempSensor();

        int16_t getTemp();
       
};