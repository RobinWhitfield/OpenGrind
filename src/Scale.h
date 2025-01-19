#pragma once

#include <SimpleKalmanFilter.h>
#include <HX711.h>

#include "Definitions.h"

class Scale
{
  private:
    Scale *scale;
    unsigned long timeLastKalman = micros();

  public:
    Scale();
    //bool hx711present; //TODOw
    void tare(uint_least8_t samples); //TODO Tare the scale on demand
    //void calibrate(uint_least16t mass); //TODO Calibrate with mass
    int16_t getMeasurement(uint_least8_t samples);
    //float getMeasurement();
};