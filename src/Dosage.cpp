#include <EEPROM.h>

#include "Dosage.h"

Dosage::Dosage() {
    EEPROM.get(eeAddress, dose1Time);
    EEPROM.get(eeAddress + sizeof(uint16_t), dose2Time);
    dose1Time = dose1Time == 0 ? DOSE1_DEFAULT_TIME : dose1Time;
    dose2Time = dose2Time == 0 ? DOSE2_DEFAULT_TIME : dose2Time;
}

void Dosage::writeToEEPROM() {
        EEPROM.put(eeAddress, dose1Time);
        EEPROM.put(eeAddress + sizeof(uint16_t), dose2Time);
}