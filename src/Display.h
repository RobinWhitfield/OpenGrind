#pragma once

//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
//#include <Adafruit_SH110X.h>
#include <Arduino.h>
#include <ss_oled.h>

#include "Definitions.h"
#include "Temperature.h"

class DISPLAYDRIVER;

class Display
{
  private:
    DISPLAYDRIVER* display;

  public:
    Display();
    void testText();
    void printProgram(uint_least8_t prog);
    //void printDose1();
    //void printDose2();
    //void printGBWDose();
    void printTime(uint16_t time, int16_t temp, int16_t mass, uint_least8_t program);
    void printStatistics(uint16_t numberDose1, uint16_t numberDose2, uint16_t numberGBWDose);
    void resetText();
    void clear();
};