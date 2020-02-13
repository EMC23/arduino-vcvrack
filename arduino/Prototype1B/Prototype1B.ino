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
#define MAX_EVENTS_PER_PATTERN 12

bool is_playing = false;
bool is_message = false;
uint8_t bank_index = 0;
uint8_t ptrn_index = 0;

struct midi_event {
  uint32_t time_tag;
  uint8_t header;
  uint8_t byte1;
  uint8_t byte2;
  uint8_t byte3;
};

struct pattern {
  struct midi_event events[MAX_EVENTS_PER_PATTERN];
  uint8_t index;
  uint8_t length;
};

struct pattern patterns[NUM_BANKS][NUM_PATTERNS_PER_BANK];

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
void add_midi_note(uint8_t bank_index, uint8_t ptrn_index, uint32_t time, uint32_t duration, uint8_t channel, uint8_t pitch, uint8_t velocity) {
  if (ptrn_index < NUM_BANKS && ptrn_index < NUM_PATTERNS_PER_BANK) {
    struct pattern* pattern_p = &patterns[bank_index][ptrn_index];
    if (pattern_p->index + 1 < MAX_EVENTS_PER_PATTERN) {
      pattern_p->events[pattern_p->index] = { time, NOTE_ON_HEADER, (uint8_t)NOTE_ON | channel, pitch, velocity };
      pattern_p->index++;
      pattern_p->length++;
      pattern_p->events[pattern_p->index] = { time, NOTE_OFF_HEADER, (uint8_t)NOTE_OFF | channel, pitch, 0 };
      pattern_p->index++;
      pattern_p->length++;
    }
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
void order_events() {
  uint8_t i, j, k, l, ptrn_length;
  midi_event swap_event;
  for (i = 0; i < NUM_BANKS; i++) {
    for (j = 0; j < NUM_PATTERNS_PER_BANK; j++) {
      struct pattern* pattern_p = &patterns[i][j];
      ptrn_length = pattern_p->length;
      for (k = 0; k < ptrn_length; ++k) {
        for (l = 0; l < ptrn_length; ++l) {
          if (pattern_p->events[k].time_tag > pattern_p->events[l].time_tag) {
            swap_event = pattern_p->events[k];
            pattern_p->events[k] = pattern_p->events[l];
            pattern_p->events[l] = swap_event;
          } 
        }
      }
    }
  }
}

/**
 * 
 */
void create_patterns() {
  create_kicks();
  order_events();
}

void clockOut96PPQN(uint32_t* tick) {

}

/**
 * 
 */
void sendMIDIMessage(uint8_t header, uint8_t byte0, uint8_t byte1, uint8_t byte2) {
  midiEventPacket_t midi_event = {header, byte0, byte1, byte2};
  MidiUSB.sendMIDI(midi_event);
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
