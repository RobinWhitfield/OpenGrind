#include "Scale.h"


HX711 HX711scale;

SimpleKalmanFilter kalmanFilter(9, 9, 0.01); //Initial values


// Tare the scale
void Scale::tare(uint8_t samples) {
  // TODO - Add logic to check for instability
  HX711scale.tare(samples);      // Tare the scale, using an average of 40 samples (~0.5s)
}

// Initialise the scale
Scale::Scale() {
  
    HX711scale.begin(SCALE_DAT, SCALE_CLK);
    while (! HX711scale.is_ready()){}       // Do nothing until scale is ready - TODO, logic in here for load cell not connected, no GBW
    HX711scale.set_scale(276.5);            // 750g load cell, output in decigrams (to be divided by 10 for display) - TODO, add calibration routine.
    HX711scale.set_average_mode();          // Set the scale to average mode
    delay(1000);                            // Delay to allow HX711 some time to stabilise
    Scale::tare(40);                        // Tare the scale
}

int16_t Scale::getMeasurement(uint_least8_t samples) {
    float rawValue = HX711scale.get_units(samples);     // Request reading from scale
    //if (rawValue >= -3 && rawValue <= 3) rawValue = 0;  // Stabilise zero reading (ignore 0.7g around zero)

    float processNoise = ((micros()-timeLastKalman));                     // Get the time since we were last here
    timeLastKalman = micros();                                            // Set the last time to now
    processNoise = processNoise / 1000000;                                // Turn that time taken into seconds
    kalmanFilter.setProcessNoise(processNoise);                           // Provide the time to the Kalman filter
    int16_t filteredValue = kalmanFilter.updateEstimate(rawValue) + 0.5;  // Add the last measurement to the Kalman filter, get value and add 0.5 so the cast to int rounds correctly
    Serial.println(processNoise, 6);
    if (filteredValue < 0) {       // if less than 0g, make zero - this assumes good zero on boot
    
      /*
      if (filteredValue < -20){     // if more than 2g below zero, re-tare - causes issues if you knock the scale, needs a time component
        delay(100);                 // wait for object to leave scale
        tare();
        herelast = millis();
        } 
      */
      //
      //filteredValue = 0;
    }
    return filteredValue;                                    // Return the filtered value
    
    
}

/*
float Scale::getMeasurement(){
  if (LoadCell.dataWaitingAsync()) { // If data, proceed
      LoadCell.updateAsync();
    }
    return LoadCell.getData();
}
*/