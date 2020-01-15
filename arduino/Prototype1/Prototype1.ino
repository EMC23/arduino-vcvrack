
const uint8_t NUM_LEDS = 8;
const uint8_t PIN_PUSH_BTN = 8;
const uint8_t PIN_ENCODER_BTN = 9;
const uint8_t PIN_ENCODER_A = 10;
const uint8_t PIN_ENCODER_B = 11;

void setup() {
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(PIN_PUSH_BTN, INPUT);
  pinMode(PIN_ENCODER_BTN, INPUT);
  pinMode(PIN_ENCODER_A, INPUT);
  pinMode(PIN_ENCODER_B, INPUT);
}

void loop() {
  
}