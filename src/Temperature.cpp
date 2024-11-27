#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "Definitions.h"
#include "Temperature.h"

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

DeviceAddress tempSensor;

TempSensor::TempSensor() {
    sensors.begin();
    sensors.getDeviceCount();
    sensors.getAddress(tempSensor, 0);
    sensors.setResolution(tempSensor, 10); // Set to 10 bit resolution (0.25C), set to 12 bit to make testing easier
    sensors.setWaitForConversion(false); // Don't wait for temp sensors
}

int16_t TempSensor::getTemp(){
    if (millis()-lastmillis > 1000 ) { // only check for temperature changes every second
        sensors.requestTemperaturesByAddress(tempSensor);
        lastmillis = millis();
    }

    float fltTemp = sensors.getTempC(tempSensor); // get temp
    int16_t temp = fltTemp*100; // change to int to reduce memory
    return temp;
}