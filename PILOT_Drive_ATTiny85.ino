#include <avr/io.h>

// *PINS ARE ATTINY85 PB PINS, NOT PHYSICAL*

const int RELAY_OUT = 4;
const int ACC_IN = 3;

int state = 0;

int wait_period = 60; // Would like to make this i2c controlled, allow the Pi to set it's own shutdown time.

void setup() {
  pinMode(RELAY_OUT, OUTPUT);
  pinMode(ACC_IN, INPUT_PULLUP); // Accessory voltage in, either high or low w/ voltage divider

  digitalWrite(RELAY_OUT, HIGH); // Turn on the relay on startup
}

void loop() {
  int new_state = digitalRead(ACC_IN);
  if (new_state != state) {
    state = new_state;
    if (state == 1) {
      int i = 0;
      while ((i < wait_period) && (digitalRead(ACC_IN) != 0)) { // I know, I know. This isn't an accurate time representatation blah blah blah - it's good enough
        delay(1000);
        i++;
        if (i == wait_period) {
          digitalWrite(RELAY_OUT, LOW);
        }
      }
    }
  }
}
