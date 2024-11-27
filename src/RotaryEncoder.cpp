#include <Encoder.h>
#include <Bounce2.h>

#include "Definitions.h"
#include "RotaryEncoder.h"

Bounce2::Button encoderPin = Bounce2::Button();

RotaryEncoder::RotaryEncoder() {
    encoder = new Encoder(ENC_CLK, ENC_DT);

    encoderPin.attach(ENC_SW, INPUT_PULLUP);
    encoderPin.interval(5);
    encoderPin.setPressedState(LOW);
}

bool RotaryEncoder::wasTurnedLeft() {
    if (encoder->read() > 0 + ENC_TOL) {
        encoder->write(0);
        return true;
    }
    return false;
}

bool RotaryEncoder::wasTurnedRight() {
    if (encoder->read() < 0 - ENC_TOL) {
        encoder->write(0);
        return true;
    }
    return false;
}

void RotaryEncoder::btnUpdate() {
    encoderPin.update();
}

bool RotaryEncoder::isPressed() {
    return encoderPin.isPressed();
}

bool RotaryEncoder::wasPressed() {
    return encoderPin.pressed();
}

bool RotaryEncoder::wasReleased() {
    return encoderPin.released();
}

bool RotaryEncoder::btnChanged() {
    return encoderPin.changed();
}

bool RotaryEncoder::wasLongPressed() {
    if (encoderPin.isPressed() && encoderPin.currentDuration() > ENC_SW_LONG_PRESS_DUR) {
        return true;
    }
    return false;
}