#define DISPLAY_ADDR 0x3C // I2C Address - use 0x3C or 0x3D depending on your display
//#define SCREENTYPE GRAPHIC // Acceptable values GRAPHIC, TEXT (not yet implemented)
#define DISPLAYDRIVER Adafruit_SH1106G // Probably Adafruit_SH1106G or Adafruit_SSD1306
#define DISPLAYWIDTH 128 // For GRAPHIC, supports 128. [For TEXT, support to be added for 20, 16]
#define DISPLAYHEIGHT 64 // For GRAPHIC, supports 64, 32 (kind of). [For TEXT, support to be added for 4,2]
#define INVERTDISPLAY false // true to invert display - generally looks horrible but can be useful for design
#define DISPLAYROTATION 0 // Rotate display. Only 0 (normal) or 2 (upside down) are supported

#define ENC_CLK 2 // Encoder CLK Input Pin
#define ENC_DT 3 // Encoder DT Input Pin
#define ENC_SW 4 // Encoder Switch Input Pin
#define ENC_TOL 2 // Clicks of encoder tolerance. Typically should be 2
#define ENC_SW_LONG_PRESS_DUR 5000 // Encoder Switch Long Press Activation Time in ms
#define GRIND_BTN 5 // Start Button Input Pin
#define SSR 6 // SSR Output Pin

#define MAX_DOSE_TIME 9900 // The maximum grind time, in milliseconds. Some displays will have issues over 9990
#define DOSE_PRECISION 10 // The precision of dose time you wish to set (in ms). 10/50 is typically fine. If your max dose time is not divisible by this, you might end up with funny numbers.

//#define SHOTSTATS // Shot counter. Comment this out in a commercial setting or you'll probably wear out the MCU EEPROM

#define DOSE1_DEFAULT_TIME 3000 // Default Dose 1 Time in ms
#define DOSE2_DEFAULT_TIME 6000 // Default Dose 2 Time in ms