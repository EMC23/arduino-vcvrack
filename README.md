# Arduino - VCV Rack

Two way communication between an Arduino board and a VCV Rack module.

## VCV Rack

### Plugin

The VCV Rack modules are part of the plugin Arduino Modules located at `/vcvrack/ArduinoModules`.

### Module

The first module is called 'Arduino MIDI'.

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

### Arduino Leonardo Mac OS Mojave

Arduino Leonardo USB port doesn't appear.<br>
Possible fix: https://arduino.stackexchange.com/questions/57126/connect-to-ch340-on-macos-mojave

```bash
sudo rm -rf /Library/Extensions/usbserial.kext
sudo rm -rf /System/Library/Extensions/usb.kext
```

Then reboot.