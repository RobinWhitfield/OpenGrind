#define DISPLAY_ADDR 0x3C // I2C Address - use 0x3C or 0x3D depending on your display
#define DISPLAYDRIVER Adafruit_SH1106G // Currently Adafruit_SH1106G or Adafruit_SSD1306
#define DISPLAYWIDTH 128 // For GRAPHIC, supports 128
#define DISPLAYHEIGHT 64 // For GRAPHIC, supports 64
#define INVERTDISPLAY false // true to invert display - generally looks horrible but can be useful for design
#define DISPLAYROTATION 0 // Rotate display. Only 0 (normal) or 2 (upside down) are supported

//---For SSOLED Lib:
#define SDA_PIN -1
#define SCL_PIN -1
// no reset pin needed
#define RESET_PIN -1
// let ss_oled find the address of our display
#define OLED_ADDR -1
#define FLIP180 0
#define INVERT 0
// Use the default Wire library
#define USE_HW_I2C 1
//----

#define ENC_CLK 2 // Encoder CLK Input Pin
#define ENC_DT 3 // Encoder DT Input Pin
#define ENC_SW 4 // Encoder Switch Input Pin
#define ENC_TOL 2 // Clicks of encoder tolerance. Typically should be 2
#define ENC_SW_LONG_PRESS_DUR 5000 // Encoder switch long press activation time in ms

#define GRIND_BTN 5 // Start/grind button Input pin
#define GRIND_BTN_LONG_PRESS_DUR 500 // Grind button long press activation time in ms (TODO)

#define SSR 6 // SSR Output pin

#define SCALE_DAT 8 // Scale Data Pin
#define SCALE_CLK 9 // Scale Clock Pin

#define ONE_WIRE_BUS 7 // One wire bus pin for DS18B20 sensor

#define MAX_DOSE_TIME 17000 // The maximum grind time, in milliseconds.
#define MAX_GBW_DOSE 500 // The maximum GBW dose in decigrams
#define DOSE_PRECISION 50 // The precision of dose time you wish to set (in ms). Min 10, 50 is usually fine. If your max dose time is not divisible by this, you might end up with funny numbers.
#define GBW_DOSE_PRECISION 10 // GBW dose precision (in decigrams, 1 = 0.1g)

#define DOSESTATS // Dose counter enabled. Comment this out in a commercial setting or you might wear out the MCU EEPROM

#define DOSE1_DEFAULT_TIME 3000 // Default Dose 1 Time in ms
#define DOSE2_DEFAULT_TIME 6000 // Default Dose 2 Time in ms
#define GBW_DEFAULT_DOSE 180 // default GBW dose in decigrams (/10 to get grams, yes it's weird, just live with it)