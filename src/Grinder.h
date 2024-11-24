#pragma once

class Button;
class Grinder
{  
    private:
        Button *button;
        int eeAddress = 100;
        //unsigned long targetTime = 0;

    public:
        Grinder();
        bool startBtnPressed();
        int getDose1Stats();
        int getDose2Stats();
        void increaseShotCounter(bool isDose1);
        void resetStats();
        unsigned long grindingStart;
        unsigned long grindingTime;
        void on();
        void off();
};