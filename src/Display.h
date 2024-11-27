#pragma once

class DISPLAYDRIVER;

class Display
{
  private:
    DISPLAYDRIVER* display;

  public:
    Display();
    void printDose1();
    void printDose2();
    void printTime(uint16_t time, int16_t temp);
    void printStatistics(uint16_t numberDose1, uint16_t numberDose2);
    void resetText();
};