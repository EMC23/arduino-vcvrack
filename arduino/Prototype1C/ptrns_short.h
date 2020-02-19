#ifndef PTRNS_SHORT_H
#define PTRNS_SHORT_H

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
#define NUM_BANKS 2
#define NUM_PATTERNS_PER_BANK 2
#define MAX_EVENTS_PER_PATTERN 24

/**
 * Short event notation to be able to fit as many events as possible
 * in the Arduino RAM memory.
 * Timer resolution is 24PPQN, so one 4 x 4 bar is 96 ticks. That's what this 
 * prototype uses, so 7 bits is enough for time.
 * 
 * Event time: 7 bits, 0 to 95 used
 * Event type: 1 bit, 0 = Note, 1 = Continuous Controller.
 * Event channel: 2 bits, first 4 of the MIDI channels.
 * 
 * Note event pitch: 5 bits, 32 / 12 = 2 octaves + 8 notes.
 * Note event velocity: 2 bits, 0 and 3 other velocities.
 * 
 * CC event controller: 2 bits, 4 controllers supported.
 * CC event value: 5 bits, 32 values instead of the regular 127.
 * 
 * 
 * In case of 32 bits event:
 * 8 bits, time 8 beats, 2 bars of 24 PPQN
 * 2 bits, type Note or CC or two others
 * 4 bits, channel 1 to 16
 * 
 * 8 bits, Note pitch
 * 8 bits, Note velocity
 * 
 * 8 bits, CC controller
 * 8 bits, CC value
 * 
 * 
 * In case of 16 and 8, 24 bits event:
 * 
 */
typedef uint32_t event_t;

typedef struct {
  event_t events[MAX_EVENTS_PER_PATTERN];
  uint8_t index;
  uint8_t num_events;
} pattern_t;

void add_note(pattern_t* pattern_p, uint8_t time, uint8_t duration, uint8_t channel, uint8_t pitch, uint8_t velocity);

/*
#ifdef __cplusplus
} // extern "C"
*/

#endif
