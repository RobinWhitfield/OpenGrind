#pragma once

class DISPLAYDRIVER;

class Display
{
  private:
    DISPLAYDRIVER* display;

  public:
    Display();
    void printSingleDose();
    void printDoubleDose();
    void printTime(double time);
    void printStatistics(int numberSingles, int numberDoubles);
};