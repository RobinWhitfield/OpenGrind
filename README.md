<p align="center">
  <img width="300" src="assets/logo.png">
  <h3 align="center">Sophisticated Coffee Grinder Timer Firmware</h3></br>
  <p align="center">
    <img alt="license" src="https://img.shields.io/badge/license-GPL-blue">
  <img alt="gitter" src="https://img.shields.io/badge/language-C/C++-blue">
  </p>
</p>
<br>

## About

This is a fork of GatCode's [OpenGrind](https://github.com/GatCode/OpenGrind), modified to be closer to the function of commercial grinders like the Mythos.



## Prerequisites

The getting started section below assumes that you are using mostly the same hardware and configuration as stated in this [Instructable](https://www.instructables.com/The-14-Sophisticated-Coffee-Grinder-Timer/).

If your screen doesn't work, it's probably SH1106x rather than SSD1306 so try changing the driver.

The DS18B20 should be connected to pin 7 with a ~5k pullup resistor.



## Getting Started

Make sure you have installed [PlatformIO](https://platformio.org/platformio-ide) and know the basics on how to use it.

1. Open the *OpenGrind* Folder in VSCode

2. Choose the correct `upload_port`corresponing to your MCU and OS in the `platformio.ini`file. This can be for example`COM3`on Windows or`/dev/ttyUSB0`on Mac or Linux.

3. Connect your microcontroller and hit the upload button --> enjoy 🎉



## Usage

The first screen you see after booting is the default screen which contains the selection for the grinding dosage (one or two cup icons).



## Customization

If you choose to alter the hardware configuration (IO pins) or you are not satisfied with the behavior of the firmware, you can find all major settings in the `src/Definitions.h`file.



## Troubleshooting

EMI is a serious problem with this kind of project, so keep wires as short as possible, and keep the electronics away from the grinder as much as possible. Shielding the electronics may be helpful.

Shielded cable to the encoder, DS18B20, and grind button are recommended. These cable shields should be connected to ground, which itself should be tied to an earth connection (and the grinder chassis if it's metal).
The LCD should be immediately adjacent to the microcontroller as i2c is not designed for long distance communication.

Strong hardware pullups on the input pins, and decoupling capacitors may also be helpful/necessary.


## Limitations

This program is provided as-is, and although care has been taken to avoid them, bugs may be present. If you find one, please submit an issue, or fix it yourself and submit a PR.


**WARNING:** 

Working with mains voltages can be dangerous so take care if you decide to replicate this project. You're responsible for your own actions.
Also take note of the fact that performing this modification on a grinder that has a warranty will cause it to no longer have a warranty.