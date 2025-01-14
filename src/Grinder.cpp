#include <Arduino.h>
#include <EEPROM.h>
#include <Bounce2.h>

#include "Definitions.h"
#include "Grinder.h"

Bounce2::Button grindPin = Bounce2::Button();

Grinder::Grinder() {
    pinMode(SSR, OUTPUT);

    grindPin.attach(GRIND_BTN, INPUT_PULLUP);
    grindPin.interval(5);
    grindPin.setPressedState(LOW);
}

/*
bool Grinder::startBtnPressed() {
    return !digitalRead(GRIND_BTN); // Negated because PULLUP
}
*/

void Grinder::btnUpdate() {
    grindPin.update();
}

bool Grinder::isPressed() {
    return grindPin.pressed();
}

bool Grinder::wasPressed() {
    return grindPin.pressed();
}

bool Grinder::wasReleased() {
    return grindPin.released();
}

bool Grinder::wasLongPressed() {
    if (grindPin.currentDuration() > GRIND_BTN_LONG_PRESS_DUR) {
        return true;
    }
    return false;
}

uint16_t Grinder::getStats(uint_least8_t i) {
    int res = 0;
    EEPROM.get(eeAddress + (i * sizeof(uint16_t)), res);
    return res;
}

#ifdef DOSESTATS
void Grinder::increaseStatsCounter(uint_least8_t whichDose) {
    switch (whichDose) {
        case 0:
            EEPROM.put(eeAddress, getStats(0) + 1);
            break;
        case 1:
            EEPROM.put(eeAddress + sizeof(uint16_t), getStats(1) + 1);
            break;
        case 2:
            EEPROM.put(eeAddress + (2 * sizeof(uint16_t)), getStats(2) + 1);
            break;
    }
}
#endif

void Grinder::resetStats() {
    for (unsigned int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);
    }
}

void Grinder::on() {
    digitalWrite(SSR, HIGH);
}

void Grinder::off() {
    digitalWrite(SSR, LOW);
}