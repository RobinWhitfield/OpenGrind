#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_SH110X.h>

#include "Definitions.h"
#include "Display.h"

//#include <Fonts/FreeMonoBold24pt7b.h>

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

Display::Display() {
    display = new DISPLAYDRIVER(DISPLAYWIDTH, DISPLAYHEIGHT, &Wire, -1);

    //    if (!display->begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR)) {
        if (!display->begin(DISPLAY_ADDR)) {
            for(;;);
        }

    display->cp437(true);
    display->clearDisplay();
    display->display();
    display->invertDisplay(INVERTDISPLAY);
    display->setRotation(DISPLAYROTATION);
}

void Display::simpleText(char text[]) {
    display->clearDisplay();
    display->setTextColor(WHITE);
    display->setCursor(0,0);
    display->setTextSize(1);
    display->print(text);
    display->display();
}

void Display::printDose1() {
    display->clearDisplay();
    display->drawBitmap(
        (display->width()  - 40) / 2,
        (display->height() - 40) / 2,
        cup, 40, 40, 1);
    display->display();
}

void Display::printDose2() {
    display->clearDisplay();
    display->drawBitmap(
        (display->width()  - 94) / 2,
        (display->height() - 40) / 2,
        cup, 40, 40, 1);
    display->drawBitmap(
        (display->width() + 14 ) / 2,
        (display->height() - 40) / 2,
        cup, 40, 40, 1);
    display->display();
}

void Display::printTime(uint16_t time) {
    time /= 10;
    display->clearDisplay();

    display->setTextColor(WHITE);

    if(DISPLAYHEIGHT == 32) {
        display->setTextSize(2);
        display->setCursor(26, 4);
    }
    else {
        display->setTextSize(4);
        display->setCursor(14, 18);
    }
    //display->print(time, time < MAX_DOSE_TIME + DOSE_PRECISION ? 2 : 1);
    int val = time / 100;
    int dec = time % 100;
    display->print(val);
    display->print(".");
    if(dec<10) { display->print("0"); }
    display->print(dec);
    if(DISPLAYHEIGHT == 32) {
        display->setTextSize(1);
        display->setCursor(100, 18);
    }
    else {
        display->setTextSize(1);
        display->setCursor(112, 39);
    }
    display->print("s");

    display->display();
}

void Display::printStatistics(uint16_t numberDose1, uint16_t numberDose2) {
    display->clearDisplay();
    display->setTextColor(WHITE);

    #ifdef SHOTSTATS
    display->setTextSize(2);
    display->setCursor(10, 4);
    display->print("D1: ");
    display->println(numberDose1);
    display->setCursor(10, 24);
    display->print("D2: ");
    display->println(numberDose2);
    #else
    display->setTextSize(1);
    display->setCursor(0, 12);
    display->println("Commercial mode");
    display->println("Stats disabled");
    #endif
    display->setTextSize(1);
    display->println();
    display->print("Hold to factory reset");
    display->display();
}
//#endif