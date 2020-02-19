
extern "C" {
  #include "ptrns_short.h"
}

pattern_t patterns[NUM_BANKS][NUM_PATTERNS_PER_BANK];

void createTestPattern() {
  uint8_t bank_index = 0;
  uint8_t pattern_index = 0;
  uint8_t channel = 10;
  uint8_t pitch = 60;
  pattern_t* pattern_p = &patterns[bank_index][pattern_index];
  pattern_p->index = 0;
  pattern_p->num_events = 0;
  add_note(pattern_p, PPQN * 0, PPQN * 0.5, channel, pitch, 100);
}

void setup() {
  createTestPattern();
  // order_events();
  // serialPrintPattern(0, 0);
}

void loop() {}
