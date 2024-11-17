#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include "Definitions.h"
#include "RotaryEncoder.h"
#include "Display.h"
#include "Dosage.h"
#include "Grinder.h"

RotaryEncoder *encoder;
Display *display;
Dosage *dosage;
Grinder *grinder;

// State Machine
enum States {CHANGE_DOSE, SET_DOSE, GRINDING, STATS};
uint8_t state = SET_DOSE;

void setup() {
  Serial.begin(9600);
  encoder = new RotaryEncoder();
  display = new Display();
  dosage = new Dosage();
  grinder = new Grinder();
}

void(* resetFunc) (void) = 0;

void loop() {
  switch(state)
  {
    case SET_DOSE:
      // change dose
      if (encoder->wasPressed()) {
        state = CHANGE_DOSE;
        break;
      }

      // start grinding
      if (grinder->startBtnPressed()) {
        state = GRINDING;
        break;
      }

      // show stats
      if (encoder->wasLongPressed()) {
        state = STATS;
        break;
      }

      if (encoder->wasTurnedLeft()) {
        dosage->dose1Time -= dosage->dose1Selected ? DOSE_PRECISION : 0.0;
        dosage->dose2Time -= dosage->dose1Selected == false ? DOSE_PRECISION : 0.0;
        dosage->dose1Time = dosage->dose1Time < 0 ? 0.00 : dosage->dose1Time;
        dosage->dose2Time = dosage->dose2Time < 0 ? 0.00 : dosage->dose2Time;

      } else if (encoder->wasTurnedRight()) {
        dosage->dose1Time += dosage->dose1Selected ? DOSE_PRECISION : 0.0;
        dosage->dose2Time += dosage->dose1Selected == false ? DOSE_PRECISION : 0.0;
        dosage->dose1Time = dosage->dose1Time < MAX_DOSE_TIME ? dosage->dose1Time : MAX_DOSE_TIME;
        dosage->dose2Time = dosage->dose2Time < MAX_DOSE_TIME ? dosage->dose2Time : MAX_DOSE_TIME;
      }

      
      display->printTime(dosage->dose1Selected ? dosage->dose1Time : dosage->dose2Time);

      break;

    case CHANGE_DOSE:
      // select dose
      if (encoder->wasTurnedLeft()) {
        dosage->dose1Selected = true;
      } else if (encoder->wasTurnedRight()) {
        dosage->dose1Selected = false;
      }

      // display dose icons
      if (dosage->dose1Selected) {
        display->printDose1();
      } else {
        display->printDose2();
      }
      state = SET_DOSE;
      break;

    case GRINDING:
      
      dosage->writeToEEPROM(); //Write dose time to EEPROM, will only write if value has changed.
      #ifdef SHOTSTATS
      grinder->increaseShotCounter(dosage->dose1Selected);
      #endif
      grinder->on(dosage->dose1Selected ? dosage->dose1Time : dosage->dose2Time);
      while (millis() < grinder->getTargetTime()) {
        display->printTime((grinder->getTargetTime() - millis()) / 1000.0);
        
      }
      grinder->off();

      display->printTime(0.0);
      delay(250); // show 0.0 on display for a longer time
      state = SET_DOSE;
      break;
 
    case STATS:
      display->printStatistics(grinder->getDose1Stats(), grinder->getDose2Stats());
      if (encoder->wasPressed()) {
        state = SET_DOSE;
      }

      unsigned long target = millis() + ENC_SW_LONG_PRESS_DUR;
      while (encoder->isPressed())
      {
        if (millis() > target) {
          display->simpleText("Factory Reset...");
          delay(2500);
          grinder->resetStats();
          resetFunc();
        }
      }
      

      break;
  }
}