#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include "Definitions.h"
#include "RotaryEncoder.h"
#include "Display.h"
#include "Dosage.h"
#include "Temperature.h"
#include "Grinder.h"

RotaryEncoder *encoder;
Display *display;
Dosage *dosage;
Grinder *grinder;
TempSensor *temperature;  

// State Machine
enum States {CHANGE_DOSE, SET_DOSE, GRINDING, STATS};
uint8_t state = SET_DOSE;
uint8_t lastState = SET_DOSE;

void setup() {
  //Serial.begin(9600);
  encoder = new RotaryEncoder();
  display = new Display();
  dosage = new Dosage();
  grinder = new Grinder();
  temperature = new TempSensor();
}

void(* resetFunc) (void) = 0;

void loop() {

  grinder->btnUpdate(); // Let the bounce library know the button states
  encoder->btnUpdate();
  int16_t temp = temperature->getTemp(); // get sensor temp

  if (!(state == 2)) { grinder->off(); } // if not grinding, grinder off

  switch(state)
  {
    case SET_DOSE:

      // show stats
      if (encoder->wasLongPressed()) {
        lastState = state;
        state = STATS;
        break;
      }

      // change dose
      if (encoder->isPressed()) {
        lastState = state;
        state = CHANGE_DOSE;
        break;
      }

      // start grinding
      if (grinder->wasPressed()) {
        lastState = state;
        state = GRINDING;
        if(!(lastState == GRINDING)){
          grinder->grindingStart = millis();
          grinder->grindingTime = dosage->dose1Selected ? dosage->dose1Time : dosage->dose2Time;
        }
        break;
      }

      // increase dose
      if (encoder->wasTurnedLeft()) {
        dosage->dose1Time -= dosage->dose1Selected ? DOSE_PRECISION : 0;
        dosage->dose2Time -= dosage->dose1Selected == false ? DOSE_PRECISION : 0;
        dosage->dose1Time = dosage->dose1Time < MAX_DOSE_TIME ? dosage->dose1Time : 0;
        dosage->dose2Time = dosage->dose2Time < MAX_DOSE_TIME ? dosage->dose2Time : 0;

      // decrease dose
      } else if (encoder->wasTurnedRight()) {
        dosage->dose1Time += dosage->dose1Selected ? DOSE_PRECISION : 0;
        dosage->dose2Time += dosage->dose1Selected == false ? DOSE_PRECISION : 0;
        dosage->dose1Time = dosage->dose1Time < MAX_DOSE_TIME ? dosage->dose1Time : MAX_DOSE_TIME;
        dosage->dose2Time = dosage->dose2Time < MAX_DOSE_TIME ? dosage->dose2Time : MAX_DOSE_TIME;
      }

      display->printTime(dosage->dose1Selected ? dosage->dose1Time : dosage->dose2Time, temp);

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

      if (millis()-grinder->grindingStart < grinder->grindingTime) {

        unsigned long testtime = millis()-grinder->grindingStart;
        display->printTime(grinder->grindingTime - testtime, temp);
        grinder->on();

        if (encoder->wasPressed()) {
        state = SET_DOSE;
        break;
        }
      break;
      }
      grinder->off();

      #ifdef DOSESTATS
      grinder->increaseStatsCounter(dosage->dose1Selected); // Add grind to 
      #endif

      display->printTime(0, temp);
      delay(250); // show 0.0 on display for a longer time
      state = SET_DOSE;
      break;
 
    case STATS:
      display->printStatistics(grinder->getDose1Stats(), grinder->getDose2Stats());
      if(!(lastState == STATS)){
        if (encoder->isPressed()) {
          break;
        }
        else {
          lastState = state;
          break;
        }
      }

      // break out if encoder turned
      if (encoder->wasTurnedLeft() || encoder->wasTurnedRight()) {
        state = SET_DOSE;
        break;
      }

      // break out into grinding if button pressed
      if (grinder->wasPressed()) {
        state = GRINDING;
        grinder->grindingStart = millis();
        grinder->grindingTime = dosage->dose1Selected ? dosage->dose1Time : dosage->dose2Time;
      }

      // reset if encoder button held
      if ((encoder->wasLongPressed())) {
        display->resetText();
        delay(1500);
        grinder->resetStats();
        resetFunc();
      }
      break;         
  }
}