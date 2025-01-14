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

// State Machine
enum States {CHANGE_DOSE, SET_DOSE, GRINDING, STATS};
uint8_t state = SET_DOSE;
uint8_t lastState = SET_DOSE;

void setup() {
  encoder = new RotaryEncoder();
  display = new Display();
  dosage = new Dosage();
  grinder = new Grinder();
  temperature = new TempSensor();
  scale = new Scale();
}

void(* resetFunc) (void) = 0;

void loop() {
  grinder->btnUpdate(); // Let the bounce library know the button states
  encoder->btnUpdate();
  int16_t temp = temperature->getTemp(); // get sensor temp
  uint16_t mass = scale->getMeasurement(); // get scale measurement

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
          switch (dosage->doseSelected) {
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
        //grinder->grindingTime = dosage->dose1Selected == 0 ? dosage->dose1Time : dosage->dose2Time;
        break;
      }

      // increase dose
      if (encoder->wasTurnedLeft()) {
        switch (dosage->doseSelected) {
            case 0:
              dosage->dose1Time -= DOSE_PRECISION;
              dosage->dose1Time = (dosage->dose1Time > MAX_DOSE_TIME) ? dosage->dose1Time : 0;
              break;
            case 1:
              dosage->dose2Time -= DOSE_PRECISION;
              dosage->dose2Time = (dosage->dose2Time > MAX_DOSE_TIME) ? dosage->dose2Time : 0;
              break;
            case 2:
              dosage->gbwDose -= GBW_DOSE_PRECISION;
              dosage->gbwDose = (dosage->gbwDose > MAX_GBW_DOSE) ? dosage->gbwDose : 0; // Rolled over? Make zero.
              break;
        }
      // decrease dose
      } else if (encoder->wasTurnedRight()) {
        switch (dosage->doseSelected) {
          case 0:
            dosage->dose1Time += DOSE_PRECISION;
            dosage->dose1Time = (dosage->dose1Time > MAX_DOSE_TIME) ? MAX_DOSE_TIME : dosage->dose1Time;
            dosage->currentDose = dosage->dose1Time;
            break;
          case 1:
            dosage->dose2Time += DOSE_PRECISION;
            dosage->dose2Time = (dosage->dose2Time > MAX_DOSE_TIME) ? MAX_DOSE_TIME : dosage->dose2Time;
            dosage->currentDose = dosage->dose2Time;
            break;
          case 2:
            dosage->gbwDose += GBW_DOSE_PRECISION;
            dosage->gbwDose = (dosage->gbwDose > MAX_GBW_DOSE) ? MAX_GBW_DOSE : dosage->gbwDose;
            dosage->currentDose = dosage->gbwDose;
            break;
        }
        /*
        dosage->dose1Time += dosage->dose1Selected ? DOSE_PRECISION : 0;
        dosage->dose2Time += dosage->dose1Selected == false ? DOSE_PRECISION : 0;
        dosage->dose1Time = dosage->dose1Time < MAX_DOSE_TIME ? dosage->dose1Time : MAX_DOSE_TIME;
        dosage->dose2Time = dosage->dose2Time < MAX_DOSE_TIME ? dosage->dose2Time : MAX_DOSE_TIME;
        */
      }
      
      display->printTime(dosage->currentDose, temp, mass);

      break;

    case CHANGE_DOSE:
      // select dose
      if (encoder->wasTurnedLeft()) {
        if (dosage->doseSelected > 0){
          dosage->doseSelected -= 1;
        } else {
          dosage->doseSelected = 0;
        }
      } else if (encoder->wasTurnedRight()) {
        if (dosage->doseSelected < 2){
          dosage->doseSelected += 1;
        } else {
          dosage->doseSelected = 2;
        }
      }

      // display dose icons
      switch (dosage->doseSelected) {
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
      grinder->increaseStatsCounter(dosage->doseSelected); // Add grind to stats
      #endif

      display->printTime(0, temp, mass);
      delay(250); // show 0.0 on display for a longer time
      state = SET_DOSE;
      break;
 
    case STATS:
      display->printStatistics(grinder->getStats(0), grinder->getStats(1));
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
        switch (dosage->doseSelected) {
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