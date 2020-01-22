#include <MIDIUSB_Defs.h>
#include <MIDIUSB.h>

const uint8_t MIDI_CHANNEL = 0;
const uint8_t MIDI_CC_BINARY = 14;
const uint8_t MIDI_CC_POTMETER = 15;
const uint8_t MIDI_CC_TYPE = 0xb0;
const uint8_t MIDI_MSG_HEADER = 0x0b;
const uint8_t NUM_LEDS = 8;
const uint8_t PIN_PUSH_BTN = 8;
const uint8_t PIN_ENCODER_BTN = 9;
const uint8_t PIN_ENCODER_A = 10;
const uint8_t PIN_ENCODER_B = 11;
const uint8_t PIN_POTMETER = A0;
int pushBtnState = 0;
int encoderBtnState = 0;
int encoderAState;
int encoderCounter = 0;
float potMeterValue = 0;
int potmeterCCValue = 0;
int potmeterLed = 0;

void setup() {
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(PIN_PUSH_BTN, INPUT);
  pinMode(PIN_ENCODER_BTN, INPUT);
  pinMode(PIN_ENCODER_A, INPUT);
  pinMode(PIN_ENCODER_B, INPUT);

  encoderAState = digitalRead(PIN_ENCODER_A);
}

void loop() {
  // receive MIDI
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

			// type = rx.byte1 & 0xf0, channel = rx.byte1 & 0xf) + 1
      if ((rx.byte1 & 0xf0) == MIDI_CC_TYPE && (rx.byte1 & 0xf) == MIDI_CHANNEL && rx.byte2 == MIDI_CC_BINARY) {
        if (rx.byte3 >= 16 && rx.byte3 < 31) {
          int ledIndex = (int)(rx.byte3 - 16) / 2;
          digitalWrite(ledIndex, rx.byte3 % 2 == 1);
        }
      }
    }
  } while (rx.header != 0);

  // send pushbutton as MIDI CC
  if (pushBtnState != digitalRead(PIN_PUSH_BTN)) {
    pushBtnState = digitalRead(PIN_PUSH_BTN);
    int pushbuttonCCValue = pushBtnState ? 9 : 8;
    sendMIDIMessage(0x0b, MIDI_CC_TYPE | MIDI_CHANNEL, MIDI_CC_BINARY, pushbuttonCCValue);
    // updateLEDs(0, pushBtnState);
  }

  // send encoder button as MIDI CC
  if (encoderBtnState != digitalRead(PIN_ENCODER_BTN)) {
    encoderBtnState = digitalRead(PIN_ENCODER_BTN);
    int encoderbuttonCCValue = encoderBtnState ? 11 : 10;
    sendMIDIMessage(0x0b, MIDI_CC_TYPE | MIDI_CHANNEL, MIDI_CC_BINARY, encoderbuttonCCValue);
    // updateLEDs(1, encoderBtnState);
  }

  // send potmeter value as MIDI CC
  if (potMeterValue != analogRead(PIN_POTMETER)) {
    potMeterValue = analogRead(PIN_POTMETER);
    if (potmeterCCValue != (int)(potMeterValue / 8)) {
      potmeterCCValue = potMeterValue / 8;
      sendMIDIMessage(0x0b, MIDI_CC_TYPE | MIDI_CHANNEL, MIDI_CC_POTMETER, potmeterCCValue);
    }
    // if (potmeterLed != (int)(potMeterValue / 128)) {
    //   potmeterLed = potMeterValue / 128;
    //   updateLEDs(potmeterLed, 1);
    // }
  }
  if (encoderAState != digitalRead(PIN_ENCODER_A)) {
    encoderAState = digitalRead(PIN_ENCODER_A);
    int encoderOutputCCValue = digitalRead(PIN_ENCODER_B) != encoderAState ? 14 : 13;
    sendMIDIMessage(0x0b, MIDI_CC_TYPE | MIDI_CHANNEL, MIDI_CC_BINARY, encoderOutputCCValue);
    // if (digitalRead(PIN_ENCODER_B) != encoderAState) {
    //   encoderCounter--;
    // } else {
    //   encoderCounter++;
    // }
    // updateLEDs(encoderCounter % 8, 1);
  }
}

void sendMIDIMessage(uint8_t header, uint8_t byte0, uint8_t byte1, uint8_t byte2) {
  midiEventPacket_t midiEvent = {header, byte0, byte1, byte2};
  MidiUSB.sendMIDI(midiEvent);
  MidiUSB.flush();
}

void updateLEDs(int index, int isOn) {
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(i, i == index ? isOn : 0);
  }
}
