#include <EEPROM.h>

#include "Dosage.h"

Dosage::Dosage() {
    EEPROM.get(eeAddress, doseSelected);
    EEPROM.get(eeAddress + sizeof(uint_least8_t), dose1Time);
    EEPROM.get(eeAddress + sizeof(uint_least8_t) + sizeof(uint16_t), dose2Time);
    EEPROM.get(eeAddress + sizeof(uint_least8_t) + 2*sizeof(uint16_t),gbwDose);
    dose1Time = dose1Time == 0 ? DOSE1_DEFAULT_TIME : dose1Time;
    dose2Time = dose2Time == 0 ? DOSE2_DEFAULT_TIME : dose2Time;
    gbwDose = gbwDose == 0 ? GBW_DEFAULT_DOSE : gbwDose;
}

void Dosage::writeToEEPROM() {
    EEPROM.put(eeAddress, doseSelected);
    EEPROM.put(eeAddress + sizeof(uint_least8_t), dose1Time);
    EEPROM.put(eeAddress + sizeof(uint_least8_t) + sizeof(uint16_t), dose2Time);
    EEPROM.put(eeAddress + sizeof(uint_least8_t) + 2*sizeof(uint16_t), gbwDose);
}