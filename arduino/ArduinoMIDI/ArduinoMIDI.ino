
#include <MIDIUSB_Defs.h>
#include <MIDIUSB.h>

const uint8_t CHANNEL = 0;
const uint8_t VELOCITY = 100;
const uint8_t PITCH = 60;
int count = 0;
bool toggle = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      
      if (rx.header == 9) {
        digitalWrite(LED_BUILTIN, HIGH);
      } else if (rx.header == 8) {
        digitalWrite(LED_BUILTIN, LOW);
      }
      
      Serial.print("Received: ");
      Serial.print(rx.header, HEX);
      Serial.print("-");
      Serial.print(rx.byte1, HEX);
      Serial.print("-");
      Serial.print(rx.byte2, HEX);
      Serial.print("-");
      Serial.println(rx.byte3, HEX);
    }
  } while (rx.header != 0);

  count++;

  if (count > 1000) {
    count = 0;
    toggle = !toggle;
    midiEventPacket_t noteEvent;
    if (toggle) {
      noteEvent = {0x09, 0x90 | CHANNEL, PITCH, VELOCITY};
    } else {
      noteEvent = {0x08, 0x80 | CHANNEL, PITCH, VELOCITY};
    }
    MidiUSB.sendMIDI(noteEvent);
    MidiUSB.flush();
  }

  delay(1);
}
