#include <Pin.hpp>

const uint8_t PIN_LED_GREEN   = PC10;
const uint8_t PIN_LED_YELLOW  = PC11;
const uint8_t PIN_LED_RED     = PC12;

Pin ledGreen( PIN_LED_GREEN );

void setup() {
  ledGreen.begin();
}

void loop() {
  ledGreen.pulse(15);
  delay(500);
}