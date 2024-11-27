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

/*
int Grinder::getDoseStats(int i) {
    static int res = 0;
    EEPROM.get(eeAddress + (i * sizeof(uint16_t)), res);
    return res;
}
*/

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

#ifdef DOSESTATS
void Grinder::increaseStatsCounter(bool isDose1) {
    if (isDose1) {
        EEPROM.put(eeAddress, getDose1Stats() + 1);
    } else {
        EEPROM.put(eeAddress + sizeof(uint16_t), getDose2Stats() + 1);
    }
}
#endif

/*
#ifdef DOSESTATS
void Grinder::increaseStatsCounter(uint8_t)) {
        EEPROM.put(eeAddress + (i* sizeof(uint16_t)), getDoseStats(uint8_t) + 1);
}
#endif
*/

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