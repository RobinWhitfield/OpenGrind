<p align="center">
  <img width="300" src="assets/logo.png">
  <h3 align="center">Open Source Coffee Grinder Firmware</h3></br>
  <p align="center">
    <img alt="license" src="https://img.shields.io/badge/license-GPL-blue">
  <img alt="gitter" src="https://img.shields.io/badge/language-C/C++-blue">
  </p>
</p>
<br>

## About

Tambaqui is an open source Coffee Grinder firmware, originally based on [OpenGrind](https://github.com/GatCode/OpenGrind), optimised and heavily modified to be more similar to how commercial grinders tend to operate, and with GBW functionality added.

## What's Tambaqui?

A type of tropical fish, that crushes fruit seeds while it eats them (although probably not coffee seeds).



## Prerequisites

The getting started section below assumes that you are using mostly the same hardware and configuration as stated in this [Instructable](https://www.instructables.com/The-14-Sophisticated-Coffee-Grinder-Timer/).

A DS18B20 should be connected to pin 7 with a ~5k pullup resistor.

A HX711 board should be connected to pins 8 (DT) & 9 (SCK)


## Getting Started

Make sure you have installed [PlatformIO](https://platformio.org/platformio-ide) and know the basics on how to use it.

1. Open the *OpenGrind* Folder in VSCode

2. Choose the correct `upload_port`corresponing to your MCU and OS in the `platformio.ini`file. This can be for example`COM3`on Windows or`/dev/ttyUSB0`on Mac or Linux.

3. Connect your microcontroller and hit the upload button --> enjoy 🎉



## Usage

The first screen you see after booting is the Dose 1 grind screen. You can turn the encoder left/right to adjust the dose time.
On the screen you will also see the temperature sensor value (will display -127 if not detected/connected) and the mass from the scale.

Pressing and rotating the encoder will allow you to change between programs (doses/GBW)

Holding the encoder will take you into the stats screen. Holding it again will reset everything to factory defaults and clear the stats.


## Customization

If you choose to alter the hardware configuration (IO pins) or you are not satisfied with the behavior of the firmware, you can find all major settings in the `src/Definitions.h`file.



## Troubleshooting

EMI is a serious problem with this kind of project, so keep wires as short as possible, and keep the electronics away from the grinder as much as possible. Shielding the electronics may be helpful.

Shielded cable to the encoder, DS18B20, and grind button are recommended. These cable shields should be connected to ground, which itself should be tied to an earth connection (and the grinder chassis if it's metal).
The LCD should ideally be immediately adjacent to the microcontroller as i2c is not designed for long distance communication.
The load cell cable should also be shielded as this is very sensitive to noise, and ideally the HX711 itself as well. The Kalman Filter (which stabilises the readings from the load cell) will handle some noise, but it could easily be overwhelmed by electrical noise from say, a large motor being turned on and off to grind coffee...

Strong hardware pullups on the input pins, and decoupling capacitors may also be helpful/necessary.


## Limitations

This program is provided as-is, and although care has been taken to avoid them, bugs may be present. If you find one, please submit an issue, or fix it yourself and submit a PR.


**WARNING:** 

Working with mains voltages can be dangerous so take care if you decide to replicate this project. You're responsible for your own actions.
Also take note of the fact that performing this modification on a grinder that has a warranty will cause it to no longer have a warranty.