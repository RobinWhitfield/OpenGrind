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

        uint16_t getStats(uint_least8_t i);
        void increaseStatsCounter(uint_least8_t whichDose);
        void resetStats();
        unsigned long grindingStart = 0;
        unsigned long grindingTime = 0;
        void on();
        void off();
};