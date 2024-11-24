#include <Arduino.h>
#include <EEPROM.h>
#include <JC_Button.h>

#include "Definitions.h"
#include "Grinder.h"

Grinder::Grinder() {
    pinMode(SSR, OUTPUT);
    pinMode(GRIND_BTN, INPUT_PULLUP);
    //button = new Button(GRIND_BTN, 100, true, true);
    //button->begin();
}

bool Grinder::startBtnPressed() {
    return !digitalRead(GRIND_BTN); // Negated because PULLUP
}

int Grinder::getDose1Stats() {
    static int res = 0;
    EEPROM.get(eeAddress, res);
    return res;
}

int Grinder::getDose2Stats() {
    static int res = 0;
    EEPROM.get(eeAddress + sizeof(uint16_t), res);
    return res;
}

#ifdef SHOTSTATS
void Grinder::increaseShotCounter(bool isDose1) {
    if (isDose1) {
        EEPROM.put(eeAddress, getDose1Stats() + 1);
    } else {
        EEPROM.put(eeAddress + sizeof(uint16_t), getDose2Stats() + 1);
    }
}
#endif

void Grinder::resetStats() {
    for (int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);
    }
}

void Grinder::on() {
    digitalWrite(SSR, HIGH);
}

void Grinder::off() {
    digitalWrite(SSR, LOW);
}