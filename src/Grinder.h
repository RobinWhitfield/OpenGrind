#pragma once

//class Button;
class Grinder
{  
    private:
        //Button *button;
        int eeAddress = 100;

    public:
        Grinder();
        //bool startBtnPressed();

        void btnUpdate();
        bool isPressed();
        bool wasPressed();
        bool wasReleased();
        bool wasLongPressed();
        bool btnChanged();

        int getDose1Stats();
        int getDose2Stats();
        void increaseStatsCounter(bool isDose1);
        void resetStats();
        unsigned long grindingStart = 0;
        unsigned long grindingTime = 0;
        void on();
        void off();
};