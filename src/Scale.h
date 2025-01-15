#pragma once

class Scale
{
  private:
    Scale *scale;

  public:
    Scale();
    //bool hx711present; //TODO

    //void tare(); //TODO Tare the scale on demand
    //void calibrate(uint_least16t mass); //TODO Calibrate with mass
    uint16_t getMeasurement(uint_least8_t samples);
};