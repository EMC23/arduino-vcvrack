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