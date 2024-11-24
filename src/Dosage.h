#pragma once
#include "Definitions.h"

class Dosage
{
  private:
    int eeAddress = 0;

  public:
    Dosage();
    bool dose1Selected = false;
    uint16_t dose1Time = DOSE1_DEFAULT_TIME;
    uint16_t dose2Time = DOSE2_DEFAULT_TIME;
    void writeToEEPROM();
};