#include <MIDIUSB_Defs.h>
#include <MIDIUSB.h>

const uint8_t MIDI_CHANNEL = 0;
const uint8_t MIDI_CC_BUTTONS = 14;
const uint8_t MIDI_CC_POTMETER = 15;
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
  // send pushbutton as MIDI CC
  if (pushBtnState != digitalRead(PIN_PUSH_BTN)) {
    pushBtnState = digitalRead(PIN_PUSH_BTN);
    int pushbuttonCCValue = pushBtnState ? 9 : 8;
    sendMIDIMessage(0x0b, 0xb0 | MIDI_CHANNEL, MIDI_CC_BUTTONS, pushbuttonCCValue);
    // updateLEDs(0, pushBtnState);
  }

  // send encoder button as MIDI CC
  if (encoderBtnState != digitalRead(PIN_ENCODER_BTN)) {
    encoderBtnState = digitalRead(PIN_ENCODER_BTN);
    int encoderbuttonCCValue = encoderBtnState ? 11 : 10;
    sendMIDIMessage(0x0b, 0xb0 | MIDI_CHANNEL, MIDI_CC_BUTTONS, encoderbuttonCCValue);
    // updateLEDs(1, encoderBtnState);
  }

  // send potmeter value as MIDI CC
  if (potMeterValue != analogRead(PIN_POTMETER)) {
    potMeterValue = analogRead(PIN_POTMETER);
    if (potmeterCCValue != (int)(potMeterValue / 8)) {
      potmeterCCValue = potMeterValue / 8;
      sendMIDIMessage(0x0b, 0xb0 | MIDI_CHANNEL, MIDI_CC_POTMETER, potmeterCCValue);
    }
    // if (potmeterLed != (int)(potMeterValue / 128)) {
    //   potmeterLed = potMeterValue / 128;
    //   updateLEDs(potmeterLed, 1);
    // }
  }
  if (encoderAState != digitalRead(PIN_ENCODER_A)) {
    encoderAState = digitalRead(PIN_ENCODER_A);
    int encoderOutputCCValue = digitalRead(PIN_ENCODER_B) != encoderAState ? 14 : 13;
    sendMIDIMessage(0x0b, 0xb0 | MIDI_CHANNEL, MIDI_CC_BUTTONS, encoderOutputCCValue);
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
