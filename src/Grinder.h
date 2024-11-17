#pragma once

class Grinder
{  
    private:
        int eeAddress = 100;
        unsigned long targetTime = 0;

    public:
        Grinder();
        bool startBtnPressed();
        int getDose1Stats();
        int getDose2Stats();
        void increaseShotCounter(bool isDose1);
        void resetStats();
        unsigned long getTargetTime();
        void on(double targetTime);
        void off();
};