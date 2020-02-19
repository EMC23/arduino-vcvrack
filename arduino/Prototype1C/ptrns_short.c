#include "ptrns_short.h"

void add_note(pattern_t* pattern_p, uint8_t time, uint8_t duration, uint8_t channel, uint8_t pitch, uint8_t velocity) {
  if (pattern_p->index + 1 < MAX_EVENTS_PER_PATTERN) {
    event_t* event = pattern_p->events[pattern_p->index];
    
    pattern_p->index++;
  }
}
