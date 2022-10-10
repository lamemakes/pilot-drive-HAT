#include <avr/io.h>
#include <TinyWireS.h>
#ifndef TWI_RX_BUFFER_SIZE
#define TWI_RX_BUFFER_SIZE ( 16 )
#endif


// *PINS ARE ATTINY85 PB PINS, NOT PHYSICAL*

volatile uint8_t waitPeriod = 30; // Time, in secs before the controller turns off the relay when ACC is absent. Default is 30s.
volatile bool newData; // I2C var to check for new data on the line
const int RELAY_OUT = 4;
const int ACC_IN = 3;
const int SLAVE_ADDR = 0x20; // Slave address, adjust this as needed, just make sure the Pi has the same value

bool accState;


// what to do when receiving data from master
void receiveEvent(uint8_t howMany)
{
  if (howMany != 1) // There should only be ONE byte containing a number which is the wait time
  {
      return;
  }
  
  waitPeriod = TinyWireS.receive();
  newData = true;
}

void setup() {
  pinMode(RELAY_OUT, OUTPUT);
  pinMode(ACC_IN, INPUT_PULLUP); // Accessory voltage in, either high or low w/ voltage divider

  //I2C Initialization
  TinyWireS.begin(SLAVE_ADDR); 
  TinyWireS.onReceive(receiveEvent); // what to do when receiving data

  digitalWrite(RELAY_OUT, HIGH); // Turn on the relay on startup
}

void loop() {
  int new_accState = digitalRead(ACC_IN);
  if (new_accState != accState) {
    accState = new_accState;
    if (accState) {
      int i = 0;
      while ((i < waitPeriod) && (digitalRead(ACC_IN) != 0)) { // I know, I know. This isn't an accurate time representatation blah blah blah - it's good enough
        tws_delay(1000);
        i++;
        if (i == waitPeriod) {
          digitalWrite(RELAY_OUT, LOW);
        }
      }
    }
  }
  TinyWireS_stop_check();
}
