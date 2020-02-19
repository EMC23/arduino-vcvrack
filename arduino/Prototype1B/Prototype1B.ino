#include "Arduino.h"
//#include <MIDIUSB_Defs.h>
//#include <MIDIUSB.h>
#include <uClock.h>

extern "C" {
  #include "patterns.h"
}

#define NUM_LEDS 8
#define PATTERN_DURATION 96

uint8_t bank_index = 0;
uint8_t ptrn_index = 0;

void serialPrintPattern(uint8_t bank_index, uint8_t ptrn_index) {
  Serial.print("Pattern: ");
  Serial.print(bank_index, DEC);
  Serial.print(", ");
  Serial.print(ptrn_index, DEC);
  Serial.println(";");
  struct pattern* pattern_p = &patterns[bank_index][ptrn_index];
  for(uint8_t i = 0; i < pattern_p->num_events; i++) {
    struct midi_event* event_p = &pattern_p->events[i];
    Serial.print("Event: ");
    Serial.print(event_p->time_tag, DEC);
    Serial.print(", ");
    Serial.print(event_p->header, DEC);
    Serial.print(", ");
    Serial.print(event_p->byte0, DEC);
    Serial.println(";");
  }
}

/**
 * 
 */
void clockOut96PPQN(uint32_t* tick) {

  // get the current pattern
  struct pattern* pattern_p = &patterns[bank_index][ptrn_index];

  // get time position within the pattern
  uint16_t ptrn_tick = (uint16_t)(*tick % PATTERN_DURATION);
  if (ptrn_tick == 0) {
    pattern_p->index = 0;
  }

  // quit if all the pattern's events have been played
  if (pattern_p->index == pattern_p->num_events) {
    return;
  }

  // get next event to play
  struct midi_event* event_p = &pattern_p->events[pattern_p->index];
  
  // process all events (if any) to happen on this tick
  uint8_t loop_count = 0;
  while (event_p->time_tag <= ptrn_tick) {
//    midiEventPacket_t event_packet = {event_p->header, event_p->byte0, event_p->byte1, event_p->byte2};
//    MidiUSB.sendMIDI(event_packet);

    // show event on panel, where note pitch 60 lights the first LED
    digitalWrite(event_p->byte1 - 60, event_p->byte1 & 0xf0 == NOTE_ON);

    // update pattern event index
    pattern_p->index++;

    // quit if this was the last event
    if (pattern_p->index == pattern_p->num_events) {
      break;
    }

    // get next event to play
    event_p = &pattern_p->events[pattern_p->index];

    // prevent infinite loop
    loop_count++;
    if (loop_count == 10) {
      break;
    }
  }
  
//   MidiUSB.flush();
}

/**
 * 
 */
//void sendMIDIMessage(struct midi_event* event_p) {
//  midiEventPacket_t event_packet = {event_p->header, event_p->byte0, event_p->byte1, event_p->byte2};
//  MidiUSB.sendMIDI(event_packet);
//  MidiUSB.flush();
//}

void setup() {
  create_testpattern();
  order_events();
  serialPrintPattern(0, 0);
  uClock.init();
  uClock.setClock96PPQNOutput(clockOut96PPQN);
  uClock.setTempo(100);
  uClock.start();
}

void loop() {}
