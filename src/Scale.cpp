#include <HX711.h>

#include "Definitions.h"
#include "Scale.h"


HX711 HX711scale;

// Initialise the scale
Scale::Scale() {
    HX711scale.begin(SCALE_DAT, SCALE_CLK);
    while (! HX711scale.is_ready()){} // Do nothing until scale is ready - TODO, logic in here for load cell not connected, no GBW
    HX711scale.set_scale(276.5);       // 750g load cell, output in decigrams (to be divided by 10 for display) - TODO, add calibration routine.
    HX711scale.set_average_mode();
    //  reset the scale to zero = 0
    HX711scale.tare();
}

//Exponential moving average filter, from https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/Exponential%20Moving%20Average/C++Implementation.html
template <uint_least8_t K, class uint_t = uint_least32_t>
class EMA {
  public:
    /// Update the filter with the given input and return the filtered output.
    uint_t operator()(uint_t input) {
        state += input;
        uint_t output = (state + half) >> K;
        state -= output;
        return output;
    }

    /// Fixed point representation of one half, used for rounding.
    constexpr static uint_t half = uint_t{1} << (K - 1);

  private:
    uint_t state = 0;
};

// This takes approximately 70ms when HX711 is at 80sps. Do not try and operate this with your HX711 at 10sps!
uint16_t Scale::getMeasurement() {
    static EMA<2> filter;
    float rawValue = HX711scale.get_units(6); // request reading (average of 6)
    int16_t intValue = rawValue; // convert to int, good enough.
    intValue = (intValue < 0) ? 0 : intValue; // if less than zero, make zero - this assumes good zero on boot - TODO, add some logic for this
    uint16_t filteredValue = filter(intValue); // EMA filter
    return filteredValue;
}