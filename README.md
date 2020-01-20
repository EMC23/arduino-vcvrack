# Arduino - VCV Rack

Two way communication between an Arduino board and a VCV Rack module.

## VCV Rack plugin

The VCV Rack modules are part of the plugin Arduino Modules located at `/vcvrack/ArduinoModules`.

## First prototype

The first prototype is an Arduino Leonardo that communicates with a VCV Rack module using MIDI. The purpose of this prototype is to understand how to interact with different components. A basic set of components are connected to the Arduino:

- 8 LEDs
  - Connected to Arduino digital pins 0 to 7
- 1 push button
  - Digital pin 8
- 1 potentiometer
  - Analog input 0
- 1 rotary encoder with push button
  - Push button on digital pin 9
  - Rotary outputs on digital pins 10 and 11

The Arduino does nothing more than to send knob and button changes to the VCV Rack module in the form of MIDI Continuous Control data, and to switch the LEDs on or off based on MIDI CC data it receives from the module.

### MIDI implementation chart

All MIDI communication uses Continuous Controllers 14 and 15 because they are 'officially' undefined in the MIDI specification.

| CC 14 |   |
| --- | --- |
| 8 | Pushbutton off |
| 9 | Pushbutton on |
| 10 | Encoder button off |
| 11 | Encoder button on |
| 12 | Encoder -1 |
| 13 | Encoder +1 |
| &nbsp; | &nbsp; |
| 16 | LED 0 off |
| 17 | LED 0 on |
| 18 | LED 1 off |
| 19 | LED 1 on |
| 20 | LED 2 off |
| 21 | LED 2 on |
| 22 | LED 3 off |
| 23 | LED 3 on |
| 24 | LED 4 off |
| 25 | LED 4 on |
| 26 | LED 5 off |
| 27 | LED 5 on |
| 28 | LED 6 off |
| 29 | LED 6 on |
| 30 | LED 7 off |
| 31 | LED 7 on |

| CC 15 |   |
| --- | --- |
| 0 - 127 | Potentiometer value |

### First prototype images

<img src="assets/img/prototype1_side_2020-01-16_1200.jpg" alt="Prototype 1" style="max-width:480px;">

<img src="assets/img/prototype1_overview_2020-01-16_1200.jpg" alt="Prototype 1" style="max-width:480px;">

<img src="assets/img/prototype1_top_2020-01-16_1200.jpg" alt="Prototype 1" style="max-width:480px;">




## Notes

### libusb

`libusb` is a library for communication with USB devices: https://libusb.info/<br />
On Mac, use Homebrew to install libusb:

```bash
brew install libusb
```

On Mac libusb will be installed at:

```
/usr/local/Cellar/libusb/<version nr, example: 1.0.21>/lib/libusb-1.0.a
```

### Connect a rotary encoder

- https://www.electroschematics.com/rotary-encoder-arduino/
- https://how2electronics.com/how-to-use-rotary-encoder-with-arduino/

### Arduino Leonardo Mac OS Mojave

Arduino Leonardo USB port doesn't appear.<br>
Possible fix: https://arduino.stackexchange.com/questions/57126/connect-to-ch340-on-macos-mojave

```bash
sudo rm -rf /Library/Extensions/usbserial.kext
sudo rm -rf /System/Library/Extensions/usb.kext
```

Then reboot.

<style>
img {
  max-width: 480px;
}
</style>
