#ifndef PATTERNS_H
#define PATTERNS_H

/*
#ifdef __cplusplus
extern "C" {
*/

#include <stdint.h>
#include <stdio.h>

#define PPQN 24
#define NOTE_ON_HEADER 0x09
#define NOTE_OFF_HEADER 0x09
#define NOTE_ON 0x90
#define NOTE_OFF 0x80
#define NUM_LEDS 8
#define MIDI_CHANNEL 0
#define NUM_BANKS 4
#define NUM_PATTERNS_PER_BANK 4
#define MAX_EVENTS_PER_PATTERN 24

struct midi_event {
  uint32_t time_tag;
  uint8_t header;
  uint8_t byte0;
  uint8_t byte1;
  uint8_t byte2;
};

struct pattern {
  struct midi_event events[MAX_EVENTS_PER_PATTERN];
  uint8_t index;
  uint8_t num_events;
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
void add_midi_note(uint8_t bank_index, uint8_t ptrn_index, uint32_t time, uint32_t duration, uint8_t channel, uint8_t pitch, uint8_t velocity);

/**
 * Create a test pattern.
 */
void create_testpattern();

/**
 * Order each pattern's events chronologically
 */
void order_events();

/**
 * Print pattern contents in the console.
 * @param bank_index Pattern bank index.
 * @param ptrn_index Pattern index.
 */
void print_pattern(uint8_t bank_index, uint8_t ptrn_index);

/*
#ifdef __cplusplus
} // extern "C"
*/

#endif
