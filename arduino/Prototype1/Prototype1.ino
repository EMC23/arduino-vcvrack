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
  if (pushBtnState != digitalRead(PIN_PUSH_BTN)) {
    pushBtnState = digitalRead(PIN_PUSH_BTN);
    updateLEDs(0, pushBtnState);
  }
  if (encoderBtnState != digitalRead(PIN_ENCODER_BTN)) {
    encoderBtnState = digitalRead(PIN_ENCODER_BTN);
    updateLEDs(1, encoderBtnState);
  }
  if (potMeterValue != analogRead(PIN_POTMETER)) {
    potMeterValue = analogRead(PIN_POTMETER);
    if (potmeterLed != (int)(potMeterValue / 128)) {
      potmeterLed = potMeterValue / 128;
      updateLEDs(potmeterLed, 1);
    }
  }
  if (encoderAState != digitalRead(PIN_ENCODER_A)) {
    encoderAState = digitalRead(PIN_ENCODER_A);
    if (digitalRead(PIN_ENCODER_B) != encoderAState) {
      encoderCounter--;
    } else {
      encoderCounter++;
    }
    updateLEDs(encoderCounter % 8, 1);
  }
}

void updateLEDs(int index, int isOn) {
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(i, i == index ? isOn : 0);
  }
}
