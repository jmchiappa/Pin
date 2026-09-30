#include <Pin.hpp>

const uint8_t PIN_LED_GREEN   = PC10;
const uint8_t PIN_LED_YELLOW  = PC11;
const uint8_t PIN_LED_RED     = PC12;
const uint8_t PIN_BUTTON      = PB5;

Pin ledGreen( PIN_LED_GREEN );
Pin ledRed( PIN_LED_RED );
Pin ledYellow( PIN_LED_YELLOW );
Pin button( PIN_BUTTON ); 

bool completed = false;
uint32_t counter = 0;

void setup() {
  Serial.begin(115200);

  button.begin();
  button.mode( INPUT );
}


void loop() {
  if( !button.isRunning() ) {
    Serial.println("Acquisition de clic pendant 5 secondes...");
    button.getEdgeCounter(5000, FALLING , onCompleted );
  }
  if( completed ) {
    Serial.print("nombre de clic :");
    Serial.println(counter);
    completed = false;
  }
}

void onCompleted( uint32_t _counter ) {
  counter = _counter;
  completed = true;
}