#include "Arduino.h"
#include <MIDIUSB_Defs.h>
#include <MIDIUSB.h>
#include <uClock.h>

// #define MIDI_MODE
#define SERIAL_MODE
#define MIDI_CLOCK 0xF8
#define MIDI_START 0xFA
#define MIDI_STOP 0xFC
#define NOTE_ON 0x90
#define NOTE_OFF 0x80
#define NUM_LEDS 8
#define MIDI_CHANNEL 0

bool isNoteOn = false;
bool isClockOn = false;

void clockOut16PPQN(uint32_t * tick) {
  if (isNoteOn) {
    sendMIDIMessage(0x09, NOTE_ON | MIDI_CHANNEL, 60, 100);
    digitalWrite(2, true);
  } else {
    sendMIDIMessage(0x08, NOTE_OFF | MIDI_CHANNEL, 60, 0);
    digitalWrite(2, false);
  }
  isNoteOn = !isNoteOn;
}

// The callback function wich will be called by Clock each Pulse of 96PPQN clock resolution.
void clockOut96PPQN(uint32_t * tick) {
  if (isClockOn) {
    digitalWrite(1, true);
  } else {
    digitalWrite(1, false);
  }
  isClockOn = !isClockOn;
}

// The callback function wich will be called when clock starts by using Clock.start() method.
void onClockStart() {
  Serial.write(MIDI_START);
}

// The callback function wich will be called when clock stops by using Clock.stop() method.
void onClockStop() {
  Serial.write(MIDI_STOP);
}

void sendMIDIMessage(uint8_t header, uint8_t byte0, uint8_t byte1, uint8_t byte2) {
  midiEventPacket_t midiEvent = {header, byte0, byte1, byte2};
  MidiUSB.sendMIDI(midiEvent);
  MidiUSB.flush();
}

void setup() {
  #ifdef MIDI_MODE
    // the default MIDI serial speed communication at 31250 bits per second
    Serial.begin(31250); 
  #endif
  #ifdef SERIAL_MODE
    // for usage with a PC with a serial to MIDI bridge
    Serial.begin(115200);
  #endif

  uClock.init();
  uClock.setClock16PPQNOutput(clockOut16PPQN);
  // Set the callback function for the clock output to send MIDI Sync message.
  uClock.setClock96PPQNOutput(clockOut96PPQN);
  // Set the callback function for MIDI Start and Stop messages.
  uClock.setOnClockStartOutput(onClockStart);  
  uClock.setOnClockStopOutput(onClockStop);
  // Set the clock BPM to 126 BPM
  uClock.setTempo(100);

  // Starts the clock, tick-tac-tick-tac...
  uClock.start();
}

void loop() {

}
