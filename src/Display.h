#pragma once

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
    void printTime(uint16_t time, int16_t temp, uint16_t mass);
    void printStatistics(uint16_t numberDose1, uint16_t numberDose2, uint16_t numberGBWDose);
    void resetText();
};