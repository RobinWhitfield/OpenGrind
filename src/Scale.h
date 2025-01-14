#pragma once

class Scale
{
  private:
    Scale *scale;

  public:
    Scale();
    //bool scalepresent;

    //void tare();
    //void calibrate(uint8_t mass);
    uint16_t getMeasurement();
};