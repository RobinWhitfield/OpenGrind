#pragma once
#include "Definitions.h"

class Dosage
{
  private:
    int eeAddress = 0;

  public:
    Dosage();
    uint_least8_t doseSelected = 2;
    uint16_t dose1Time = DOSE1_DEFAULT_TIME;
    uint16_t dose2Time = DOSE2_DEFAULT_TIME;
    uint16_t gbwDose = GBW_DEFAULT_DOSE;
    uint16_t currentDose = 0;
    void writeToEEPROM();
};