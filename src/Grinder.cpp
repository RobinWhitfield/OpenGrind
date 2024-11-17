#include <Arduino.h>
#include <EEPROM.h>

#include "Definitions.h"
#include "Grinder.h"

Grinder::Grinder() {
    pinMode(SSR, OUTPUT);
    pinMode(START_BTN, INPUT_PULLUP);
}

bool Grinder::startBtnPressed() {
    return !digitalRead(START_BTN); // Negated because PULLUP
}

int Grinder::getDose1Stats() {
    static int res = 0;
    EEPROM.get(eeAddress, res);
    return res;
}

int Grinder::getDose2Stats() {
    static int res = 0;
    EEPROM.get(eeAddress + sizeof(int), res);
    return res;
}

#ifdef SHOTSTATS
void Grinder::increaseShotCounter(bool isDose1) {
    if (isDose1) {
        EEPROM.put(eeAddress, getDose1Stats() + 1);
    } else {
        EEPROM.put(eeAddress + sizeof(int), getDose2Stats() + 1);
    }
}
#endif

void Grinder::resetStats() {
    for (int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);
    }
}

unsigned long Grinder::getTargetTime() {
    return targetTime;
}

void Grinder::on(double targetTime) {
    this->targetTime = millis() + targetTime * 1000;
    digitalWrite(SSR, HIGH);

}

void Grinder::off() {
    digitalWrite(SSR, LOW);
}