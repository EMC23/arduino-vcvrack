#include "patterns.h"

void add_midi_note(uint8_t bank_index, uint8_t ptrn_index, uint32_t time, uint32_t duration, uint8_t channel, uint8_t pitch, uint8_t velocity) {
  if (ptrn_index < NUM_BANKS && ptrn_index < NUM_PATTERNS_PER_BANK) {
    struct pattern* pattern_p = &patterns[bank_index][ptrn_index];
    if (pattern_p->index + 1 < MAX_EVENTS_PER_PATTERN) {
      struct midi_event note_on_event = { time, NOTE_ON_HEADER, (uint8_t)NOTE_ON | channel, pitch, velocity };
      pattern_p->events[pattern_p->index] = note_on_event;
      pattern_p->index++;
      pattern_p->num_events++;
      struct midi_event note_off_event = { time + duration, NOTE_OFF_HEADER, (uint8_t)NOTE_OFF | channel, pitch, 0 };
      pattern_p->events[pattern_p->index] = note_off_event;
      pattern_p->index++;
      pattern_p->num_events++;
    }
  }
}

void create_testpattern() {
  uint8_t bank = 0;
  uint8_t pattern = 0;
  uint8_t channel = 10;
  uint8_t pitch = 60;
  add_midi_note(bank, pattern, PPQN * 0, PPQN * 0.5, channel, pitch, 100);
  add_midi_note(bank, pattern, PPQN * 1, PPQN * 0.5, channel, pitch, 100);
  add_midi_note(bank, pattern, PPQN * 2, PPQN * 0.5, channel, pitch, 100);
  add_midi_note(bank, pattern, PPQN * 3, PPQN * 0.5, channel, pitch, 100);
  add_midi_note(bank, pattern, PPQN * 3.75, PPQN * 0.1, channel, pitch, 50);

  add_midi_note(bank, pattern, PPQN * 0.5, PPQN * 0.5, channel, pitch + 1, 100);
  add_midi_note(bank, pattern, PPQN * 1.5, PPQN * 0.5, channel, pitch + 1, 100);
  add_midi_note(bank, pattern, PPQN * 2.5, PPQN * 0.5, channel, pitch + 1, 100);
  add_midi_note(bank, pattern, PPQN * 3.5, PPQN * 0.5, channel, pitch + 1, 100);

  add_midi_note(bank, pattern, PPQN * 1, PPQN * 0.5, channel, pitch + 2, 100);
  add_midi_note(bank, pattern, PPQN * 3, PPQN * 0.5, channel, pitch + 2, 100);
}

void order_events() {
  uint8_t i, j, k, l, ptrn_length;
  struct midi_event swap_event;
  for (i = 0; i < NUM_BANKS; i++) {
    for (j = 0; j < NUM_PATTERNS_PER_BANK; j++) {
      struct pattern* pattern_p = &patterns[i][j];
      ptrn_length = pattern_p->num_events;
      for (k = 0; k < ptrn_length; k++) {
        for (l = 0; l < ptrn_length; l++) {
          if (pattern_p->events[l].time_tag > pattern_p->events[k].time_tag) {
            swap_event = pattern_p->events[k];
            pattern_p->events[k] = pattern_p->events[l];
            pattern_p->events[l] = swap_event;
          } 
        }
      }
    }
  }
}

void print_pattern(uint8_t bank_index, uint8_t ptrn_index) {
  printf("Pattern: %i, %i:\n", bank_index, ptrn_index);
  struct pattern* pattern_p = &patterns[bank_index][ptrn_index];
  for(uint8_t i = 0; i < pattern_p->num_events; i++) {
    struct midi_event* event_p = &pattern_p->events[i];
    printf("Event: %i, %i, %i, %i, %i\n", event_p->time_tag, event_p->header, event_p->byte0, event_p->byte1, event_p->byte2);
  }
}
