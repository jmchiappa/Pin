#include <Pin.hpp>

const uint8_t PIN_LED_GREEN   = PC10;
const uint8_t PIN_LED_YELLOW  = PC11;
const uint8_t PIN_LED_RED     = PC12;

Pin ledGreen( PIN_LED_GREEN );
Pin ledRed( PIN_LED_RED );
Pin ledYellow( PIN_LED_YELLOW );

void setup() {
  ledGreen.begin();
  ledRed.begin();
  ledYellow.begin();
  ledGreen.blink( 500, 3);
  ledRed.blink( 500, 5);
  ledRed.waitUntilEndOfBlink();
  ledYellow.blink( 15, 500, 10);
}

void loop() {}