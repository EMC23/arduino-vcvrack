#include "Arduino.h"
#include <MIDIUSB_Defs.h>
#include <MIDIUSB.h>
#include <uClock.h>


#define PPQN 96
#define NOTE_ON_HEADER 0x09
#define NOTE_OFF_HEADER 0x09
#define NOTE_ON 0x90
#define NOTE_OFF 0x80
#define NUM_LEDS 8
#define MIDI_CHANNEL 0
#define NUM_BANKS 4
#define NUM_PATTERNS_PER_BANK 4
#define MAX_EVENTS_PER_PATTERN 100

bool is_playing = false;
bool is_message = false;
uint8_t bank_index = 0;
uint8_t ptrn_index = 0;

struct midiEvent {
  uint32_t time_tag;
  uint8_t header;
  uint8_t byte1;
  uint8_t byte2;
  uint8_t byte3;
};

struct midiEvent patterns[NUM_BANKS][NUM_PATTERNS_PER_BANK][MAX_EVENTS_PER_PATTERN];
int pattern_pointers[NUM_BANKS][NUM_PATTERNS_PER_BANK];

// struct midiEvent kickPattern[] = {100,0,0,0,100,0,0,0,100,0,0,0,100,0,50,0};
// struct midiEvent kickPattern[] = {
//   { 0, NOTE_ON, 60, 100 }
// };

/**
 * 
 */
void init_pattern_pointers() {
  for (int i = 0; i < NUM_BANKS; i++) {
    for (int j = 0; j < NUM_PATTERNS_PER_BANK; j++) {
      pattern_pointers[i][j] = 0;
    }
  }
}

/**
 * Add a MIDI note on and -off to a pattern 
 * @param bank Pattern bank index.
 * @param pattern Pattern index.
 * @param time Note start time in 96PPQN.
 * @param duration Note duration in 96PPQN.
 * @param channel Note channel.
 * @param pitch Note pitch.
 * @param velocity Note velocity.
 */
void add_midi_note(uint8_t bank, uint8_t pattern, uint32_t time, uint32_t duration, uint8_t channel, uint8_t pitch, uint8_t velocity) {
  int event_index = pattern_pointers[bank][pattern];
  if (bank < NUM_BANKS && pattern < NUM_PATTERNS_PER_BANK && event_index + 1 < MAX_EVENTS_PER_PATTERN) {
    patterns[bank][pattern][event_index] = { time, NOTE_ON_HEADER, (uint8_t)NOTE_ON | channel, pitch, velocity };
    pattern_pointers[bank][pattern]++;
    patterns[bank][pattern][event_index] = { time, NOTE_OFF_HEADER, (uint8_t)NOTE_OFF | channel, pitch, 0 };
    pattern_pointers[bank][pattern]++;
  }
}

/**
 * 
 */
void create_kicks() {
  uint8_t bank = 0;
  uint8_t pattern = 0;
  uint8_t channel = 10;
  uint8_t pitch = 60;
  add_midi_note(bank, pattern, PPQN * 0, 48, channel, pitch, 100);
  add_midi_note(bank, pattern, PPQN * 1, 48, channel, pitch, 100);
  add_midi_note(bank, pattern, PPQN * 2, 48, channel, pitch, 100);
  add_midi_note(bank, pattern, PPQN * 3, 48, channel, pitch, 100);
  add_midi_note(bank, pattern, PPQN * 3.75, 48, channel, pitch, 50);

  pattern = 1;
}

/**
 * Order each pattern's events chronologically
 */
void order_events() {}

/**
 * 
 */
void create_patterns() {
  init_pattern_pointers();
  create_kicks();
  order_events();
}

void clockOut96PPQN(uint32_t* tick) {

}

// void clockOut16PPQN(uint32_t* tick) {
//   is_message = false;
//   if (is_playing) {
//     is_playing = false;
//     midiEventPacket_t midiEvent = {0x08, NOTE_OFF | MIDI_CHANNEL, 60, 0};
//     MidiUSB.sendMIDI(midiEvent);
//     digitalWrite(2, false);
//     is_message = true;
//   }
//   uint32_t index = *tick % PATTERN_LENGTH;
//   if (pattern[index] > 0) {
//     is_playing = true;
//     midiEventPacket_t midiEvent = {0x09, NOTE_ON | MIDI_CHANNEL, 60, pattern[index]};
//     MidiUSB.sendMIDI(midiEvent);
//     digitalWrite(2, true);
//     is_message = true;
//   }
//   if (is_message) {
//     MidiUSB.flush();
//   }
// }

/**
 * 
 */
void sendMIDIMessage(uint8_t header, uint8_t byte0, uint8_t byte1, uint8_t byte2) {
  midiEventPacket_t midiEvent = {header, byte0, byte1, byte2};
  MidiUSB.sendMIDI(midiEvent);
  MidiUSB.flush();
}

void setup() {
  create_patterns();
  uClock.init();
  uClock.setClock96PPQNOutput(clockOut96PPQN);
  uClock.setTempo(100);
  uClock.start();
}

void loop() {}
