
#include <MIDIUSB_Defs.h>
#include <MIDIUSB.h>

const uint8_t CHANNEL = 0;
const uint8_t VELOCITY = 100;
const uint8_t PITCH = 60;

void setup() {
}

void loop() {
  midiEventPacket_t noteOn = {0x09, 0x90 | CHANNEL, PITCH, VELOCITY};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();

  delay(1000);

  midiEventPacket_t noteOff = {0x08, 0x80 | CHANNEL, PITCH, VELOCITY};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();

  delay(1000);
}
