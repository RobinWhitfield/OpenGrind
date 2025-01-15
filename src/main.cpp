#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include "Definitions.h"
#include "RotaryEncoder.h"
#include "Display.h"
#include "Dosage.h"
#include "Temperature.h"
#include "Grinder.h"
#include "Scale.h"

RotaryEncoder *encoder;
Display *display;
Dosage *dosage;
Grinder *grinder;
TempSensor *temperature;
Scale *scale;

// State machine for modes of operation
enum States : uint_least8_t {SELECT_PROGRAM, SET_DOSE, GRINDING, STATS};
uint8_t state = SET_DOSE;
uint8_t lastState = SET_DOSE;


enum Programs : uint_least8_t {DOSE1, DOSE2, GBWMODE};

void setup() {
  encoder = new RotaryEncoder();
  display = new Display();
  dosage = new Dosage();
  grinder = new Grinder();
  temperature = new TempSensor();
  scale = new Scale();
}

void(* resetFunc) (void) = 0;

void running() {
  grinder->btnUpdate(); // Let the bounce library know the button states
  encoder->btnUpdate();
  int16_t temp = temperature->getTemp(); // get sensor temp
  uint16_t mass = scale->getMeasurement(6); // get scale measurement, 6 samples

  if (!(state == GRINDING)) { grinder->off(); } // if not grinding, grinder off

  switch(state)
  {
    case SET_DOSE:

      // show stats
      if (encoder->wasLongPressed()) {
        lastState = state;
        state = STATS;
        break;
      }

      // select program ()
      if (encoder->isPressed()) {
        lastState = state;
        state = SELECT_PROGRAM;
        break;
      }

      // start grinding
      if (grinder->wasPressed()) {
        lastState = state;
        state = GRINDING;
        if(!(lastState == GRINDING)){
          grinder->grindingStart = millis();
          switch (dosage->programSelected) {
            case 0:
              grinder->grindingTime = dosage->dose1Time;
              break;
            case 1:
              grinder->grindingTime = dosage->dose2Time;
              break;
            case 2:
              grinder->grindingTime = dosage->gbwDose;
              break;
          }
        }
        break;
      }

    switch (dosage->programSelected) {
      case DOSE1:
        if (encoder->wasTurnedLeft()) {
          dosage->dose1Time -= DOSE_PRECISION;
          dosage->dose1Time = (dosage->dose1Time < 0) ? 0 : dosage->dose1Time;
          dosage->currentDose = dosage->dose1Time;
          break;
        } else if (encoder->wasTurnedRight()) {
          dosage->dose1Time += DOSE_PRECISION;
          dosage->dose1Time = (dosage->dose1Time > MAX_DOSE_TIME) ? MAX_DOSE_TIME : dosage->dose1Time;
          dosage->currentDose = dosage->dose1Time;
          break;
        } else {
          dosage->currentDose = dosage->dose1Time;
          break;
        }
      case DOSE2:
        if (encoder->wasTurnedLeft()) {
          dosage->dose2Time -= DOSE_PRECISION;
          dosage->dose2Time = (dosage->dose2Time < 0) ? 0 : dosage->dose2Time;
          dosage->currentDose = dosage->dose2Time;
          break;
        } else if (encoder->wasTurnedRight()) {
          dosage->dose2Time += DOSE_PRECISION;
          dosage->dose2Time = (dosage->dose2Time > MAX_DOSE_TIME) ? MAX_DOSE_TIME : dosage->dose2Time;
          dosage->currentDose = dosage->dose2Time;
          break;
        } else {
        dosage->currentDose = dosage->dose2Time;
        break;
        }
      case GBWMODE:
        if (encoder->wasTurnedLeft()) {
          dosage->gbwDose -= GBW_DOSE_PRECISION;
          dosage->gbwDose = (dosage->gbwDose < 0) ? 0 : dosage->gbwDose;
          dosage->currentDose = dosage->gbwDose;
          break;  
        } else if (encoder->wasTurnedRight()) {
          dosage->gbwDose += GBW_DOSE_PRECISION;
          dosage->gbwDose = (dosage->gbwDose > MAX_GBW_DOSE) ? MAX_GBW_DOSE : dosage->gbwDose;
          dosage->currentDose = dosage->gbwDose;
          break;
        }
        else {
          dosage->currentDose = dosage->gbwDose;
          break;
        }

    }

      display->printTime(dosage->currentDose, temp, mass);

      break;

    case SELECT_PROGRAM:
      // select dose
      if (encoder->wasTurnedLeft()) {
        if (dosage->programSelected > 0){
          dosage->programSelected -= 1;
        } else {
          dosage->programSelected = 0;
        }
        dosage->writeToEEPROM(); //Write values to EEPROM on change, uses EEPROM put so won't wear out EEPROM.
      } else if (encoder->wasTurnedRight()) {
        if (dosage->programSelected < 2){
          dosage->programSelected += 1;
        } else {
          dosage->programSelected = 2;
        }
        dosage->writeToEEPROM(); //Write values to EEPROM on change, uses EEPROM put so won't wear out EEPROM.
      }
      display->printProgram(dosage->programSelected);

/*
      // display dose icons
      switch (dosage->programSelected) {
        case 0:
          display->printDose1();
          break;
        case 1:
          display->printDose2();
          break;
        case 2:
          display->printGBWDose();
          break;
      }
      */
      


      state = SET_DOSE;
      break;

    case GRINDING:
      dosage->writeToEEPROM(); //Write dose time to EEPROM, will only write if value has changed.

      if (millis()-grinder->grindingStart < grinder->grindingTime) {

        unsigned long testtime = millis()-grinder->grindingStart;
        display->printTime(grinder->grindingTime - testtime, temp, mass);
        grinder->on();

        if (encoder->wasPressed()) {
        state = SET_DOSE;
        break;
        }
      break;
      }
      grinder->off();

      #ifdef DOSESTATS
      grinder->increaseStatsCounter(dosage->programSelected); // Add grind to stats
      #endif

      display->printTime(0, temp, mass);
      delay(250); // show 0.0 on display for a longer time
      state = SET_DOSE;
      break;
 
    case STATS:
      display->printStatistics(grinder->getStats(0), grinder->getStats(1), grinder->getStats(2));
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
        switch (dosage->programSelected) {
          case 1:
            grinder->grindingStart = millis();
            grinder->grindingTime = dosage->dose1Time;
            break;
          case 2:
            grinder->grindingStart = millis();
            grinder->grindingTime = dosage->dose2Time;
            break;
          case 3:
            // Who knows what to do...
            break;
        }
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

void loop(){
  while(1){
    running();
  }
}