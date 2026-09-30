#include <Pin.hpp>

Pin ledGreen( LED_BUILTIN );

void setup() {
  ledGreen.begin();
}

void loop() {
  ledGreen.toggle();
  delay(500);
}