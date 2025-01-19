#include "Display.h"

SSOLED ssoled;

char szTemp[200];

/*
static const unsigned char PROGMEM cup[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC1, 0xC1, 0x80, 0x00, 0x01, 0x83, 0x83, 0x00, 0x00, 0x01,
    0x03, 0x03, 0x00, 0x00, 0x01, 0x03, 0x03, 0x00, 0x00, 0x01, 0x81, 0x83, 0x80, 0x00, 0x00, 0xC1,
    0xC1, 0x80, 0x00, 0x00, 0x60, 0xE0, 0xC0, 0x00, 0x00, 0x30, 0x60, 0x60, 0x00, 0x00, 0x30, 0x30,
    0x20, 0x00, 0x00, 0x30, 0x30, 0x20, 0x00, 0x00, 0x70, 0x60, 0xE0, 0x00, 0x00, 0xE0, 0xC1, 0xC0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xE0, 0x0F,
    0xFF, 0xFF, 0xFF, 0xF0, 0x0F, 0xFF, 0xFF, 0xFE, 0x18, 0x0F, 0xFF, 0xFF, 0xFE, 0x08, 0x0F, 0xFF,
    0xFF, 0xFE, 0x08, 0x0F, 0xFF, 0xFF, 0xFE, 0x08, 0x0F, 0xFF, 0xFF, 0xFE, 0x08, 0x0F, 0xFF, 0xFF,
    0xFE, 0x08, 0x0F, 0xFF, 0xFF, 0xFE, 0x18, 0x0F, 0xFF, 0xFF, 0xFF, 0xF0, 0x0F, 0xFF, 0xFF, 0xF8,
    0x00, 0x0F, 0xFF, 0xFF, 0xF8, 0x00, 0x0F, 0xFF, 0xFF, 0xF8, 0x00, 0x0F, 0xFF, 0xFF, 0xF8, 0x00,
    0x0F, 0xFF, 0xFF, 0xF8, 0x00, 0x07, 0xFF, 0xFF, 0xF0, 0x00, 0x07, 0xFF, 0xFF, 0xF0, 0x00, 0x03,
    0xFF, 0xFF, 0xE0, 0x00, 0x01, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x7F,
    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
*/


Display::Display() {

    //SSOLED initialisation
    int rc;
    rc = oledInit(&ssoled, OLED_128x64, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L);       // Standard HW I2C bus at 400Khz

    if (rc != OLED_NOT_FOUND)
    {
        char *msgs[] =
        {
          (char *)"SSD1306 @ 0x3C",
          (char *)"SSD1306 @ 0x3D",
          (char *)"SH1106 @ 0x3C",
          (char *)"SH1106 @ 0x3D",
          (char *)"SH1107 @ 0x3C",
          (char *)"SH1107 @ 0x3D"
        };

        oledFill(&ssoled, 0, 1);
        oledWriteString(&ssoled, 0, 30, 0, (char *)"Tambaqui", FONT_NORMAL, 0, 1);
        oledWriteString(&ssoled, 0, 12, 2, (char *)"Open Grinder", FONT_NORMAL, 0, 1);
        oledWriteString(&ssoled, 0, 0, 7, (char *)"OLED:  ", FONT_SMALL, 0, 1);
        oledWriteString(&ssoled, 0, -1, -1, msgs[rc], FONT_SMALL, 0, 1);
        delay(1000);
        clear();
    }
    else {
        Serial.println(F("No display found, check connections and reboot"));
        while(1){
            ;;          // Halt
        }
    }
}

void Display::resetText() {

   oledFill(&ssoled, 0, 1);
   oledWriteString(&ssoled, 0, 0, 0, (char *)"Factory reset...", FONT_NORMAL, 0, 1);
}

void Display::clear(){
    oledFill(&ssoled, 0, 1);
}

void Display::printProgram(uint_least8_t prog){

    switch(prog){
        case 0: //Dose 1
            oledWriteString(&ssoled, 0, 33, 1, (char *)" Dose 1 ", FONT_SMALL, 1, 1);
        break;
        case 1: //Dose 2
            oledWriteString(&ssoled, 0, 33, 1, (char *)" Dose 2 ", FONT_SMALL, 1, 1);
        break;
        case 2: //Dose 3
            oledWriteString(&ssoled, 0, 33, 1, (char *)"GBW Mode", FONT_SMALL, 1, 1);
        break;
    }
}
// Report continuous free RAM (AVR only), as per https://docs.arduino.cc/learn/programming/memory-guide/#measuring-memory-usage-in-arduino-boards
uint_least16_t freeRam() {

  extern uint_least16_t __heap_start,*__brkval;

  uint_least16_t v;

  return (uint_least16_t)&v - (__brkval == 0  ? (uint_least16_t)&__heap_start : (uint_least16_t) __brkval);  

}

void Display::printTime(uint16_t time, int16_t temp, int16_t mass, uint_least8_t program) {
    static uint16_t lasttime;
    static int16_t lasttemp;
    static int16_t lastmass;
    if (time == lasttime && temp == lasttemp && mass == lastmass){}
    else {
        time /= 10;

        int16_t val = temp / 100;
        int16_t dec = temp % 100;
        if(dec<0) { dec -= (2*dec); } // if negative temp, invert the decimal
        if(dec < 10) { sprintf(szTemp, "%d.0%dc  ", val, dec); }
        else { sprintf(szTemp, "%d.%dc  ", val, dec); }
        oledWriteString(&ssoled, 0, 0, 7, szTemp, FONT_SMALL, 0, 1);

        val = mass / 10;
        dec = mass % 10;
        if(dec<0) { dec -= (2*dec); } // if negative mass, invert the decimal
        sprintf(szTemp, "%d.%dg  ", val, dec);
        oledWriteString(&ssoled, 0, 60, 7, szTemp, FONT_SMALL, 0, 1);

        val = time / 100;
        dec = time % 100;
        if(dec < 10) { sprintf(szTemp, "%d.0%ds  ", val, dec); }
        else { sprintf(szTemp, "%d.%ds  ", val, dec); }  
        oledWriteString(&ssoled, 0, 20, 3, szTemp, FONT_12x16, 0, 2);

        switch(program){
            case 0:
                oledWriteString(&ssoled, 0, 33, 1, (char *)" Dose 1 ", FONT_SMALL, 0, 1);
            break;
            case 1:
                oledWriteString(&ssoled, 0, 33, 1, (char *)" Dose 2 ", FONT_SMALL, 0, 1);    
            break;
            case 2:
                oledWriteString(&ssoled, 0, 33, 1, (char *)"GBW Mode", FONT_SMALL, 0, 1);
            break;
        }
        //sprintf(szTemp, "Free SRAM: %d bytes", freeRam());
        //oledWriteString(&ssoled, 0, 0, 6, szTemp, FONT_SMALL, 0, 1);


       
        mass = lastmass;
        time = lasttime;
        temp = lasttemp;
    }
}

void Display::printStatistics(uint16_t numberDose1, uint16_t numberDose2, uint16_t numberGBWDose) {

    #ifdef DOSESTATS
    
    sprintf(szTemp, "D1: %d", numberDose1);
    oledWriteString(&ssoled, 0, 0, 2, szTemp, FONT_SMALL, 0, 1);
    sprintf(szTemp, "D2: %d", numberDose2);
    oledWriteString(&ssoled, 0, 0, 3, szTemp, FONT_SMALL, 0, 1);
    sprintf(szTemp, "BW: %d", numberGBWDose);
    oledWriteString(&ssoled, 0, 0, 4, szTemp, FONT_SMALL, 0, 1);
    #else

    oledWriteString(&ssoled, 0, 0, 1, (char*)"Commercial Mode", FONT_SMALL, 0, 1);
    oledWriteString(&ssoled, 0, 0, 2, (char*)"Stats disabled", FONT_SMALL, 0, 1);
    #endif
    oledWriteString(&ssoled, 0, 0, 7, (char*)"Hold to reset", FONT_SMALL, 0, 1);
}