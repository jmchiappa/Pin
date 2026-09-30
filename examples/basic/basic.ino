#include <Pin.hpp>

const uint8_t PIN_LED_GREEN   = PC10;
const uint8_t PIN_LED_YELLOW  = PC11;
const uint8_t PIN_LED_RED     = PC12;

Pin ledGreen( PIN_LED_GREEN );

void setup() {
  ledGreen.begin();
  ledGreen.setDuration(25,500);
  ledGreen.start();
  delay(5000);
  ledGreen.setDuration(100,1000);
  delay(3000);
  ledGreen.stop( LOW );
}

void loop() {
  static uint8_t cnt;
  ledGreen.analog( cnt++ );
  delay(25);
}