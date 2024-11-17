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
    void printTime(double time);
    void printStatistics(int numberDose1, int numberDose2);
    void simpleText(char text[]);
};