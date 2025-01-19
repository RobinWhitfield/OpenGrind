#pragma once

#include <Bounce2.h>
#include <Encoder.h>

#include "Definitions.h"

class Encoder;

class RotaryEncoder
{
  private:
    Encoder *encoder;

  public:
    RotaryEncoder();

    bool wasTurnedLeft();
    bool wasTurnedRight();
    
    void btnUpdate();
    bool isPressed();
    bool wasPressed();
    bool wasReleased();
    bool wasLongPressed();
    bool btnChanged();
};