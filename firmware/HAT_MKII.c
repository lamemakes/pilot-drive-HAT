#include <avr/io.h>PORTB
#include <avr/interrupt.h>
#include <util/delay.h>
#include "HAT_MKII.h"


/*
PB4 = MOSFET control (high is powered, low kills the circuit)   | Output
PB3 = Accessory Wire Optocoupler (pulled low when active)       | Input
PB2 = SCL                                                       | Input
PB1 = Status LED                                                | Output
PB0 = SDA                                                       | Input

INIT_DDRB shifts bits accordingly to input/output.
*/


void led_blink(int ledPb, int times){
    for (int i = 0; i < times; i++){
        PORTB ^= (1 << ledPb);
        _delay_ms(500);
        PORTB ^= (1 << ledPb);
        _delay_ms(500);
    }
}

void initialize()
{
    // Setup the needed PILOT Drive HAT pins
    DDRB = initDDRB;
    PORTB = (1 << FET_CTL) | (1 << ACC_OPTO); // Pullup ACC optocoupler pin & power the FET.

    // Setup I2C in the USI interface
    USICR = (1 << USIWM1) | (1 << USICS0) | (1 << USICS1);  // Set USI interface to I2C, and clock to external w negative edge sampling.
    //DDRB &= (1 << 2) | (1 << 0); // SDA & SCL direction as input

    led_blink(STATUS_LED, 1);
}

void send_ack() {
    USISR |= (1 << USIOIF);
    DDRB |= (1 << 0);  // Initialize the SDA Data Direction Bit
    int clkMask = (1 << USICNT3) | (1 << USICNT2) | (1 << USICNT1) | (1 << USICNT0);
    if (!byte) {    // If this is the first byte...
        USISR &= clkMask;
        while ((USISR & clkMask) <= (1 << USICNT2)) { // While the counter is <= 4, BUT greater than 2. An extra clock cycle is missed with the address bits, not sure why? Maybe start cond?
            if (USISR & clkMask >= (1 << USICNT1)){
                PORTB |= (1 << SDA); // Drive SDA
            }
        }
    } else if (byte > 0) {
        USISR &= clkMask;
        while ((USISR & clkMask) < (1 << 0)) { // While the counter is < 1
            PORTB |= (1 << SDA); // Drive SDA
        }
    }
    //PORTB &= ~(1 << 0); // Stop SDA drive & change data direction
    DDRB &= ~(1 << 0);
    USISR = 0; // Reset counter
}

void recieve_data(){
    int dataIn = USIBR / 2; // It appears the external clock is sampled on BOTH (pos/neg) edges, thus gets double the intended value?
    if (!byte && (dataIn == SLAVE_ADDR)){ // If this is the first byte & the buffer == slave address...
        read_data = 1;  // Set data read to true.
        send_ack();
    } else if (byte){
        timeout = dataIn;
        send_ack();
        read_data = 0;
        data_recieved = 1;
    }

}

int main (void){

    initialize();

    while(1){
        if ((USISR & (1 << 7)) == (1 << 7)){    // Check if the 7th bit of USISR (start condition register) is a 1
            USISR = (1 << 7); // Clear start condition flag & reset counter
            while ((USISR & (1 << 5)) != (1 << 5)){ // While theres no stop (USIPF) condition...
                if ((!byte) || ((2 > byte > 0) && (read_data))){    // Only move to recieve if the first byte is being set (addressing) or if the address has already been read & validated.
                    if ((USISR & (1 << 6)) == (1 << 6)) {   // Check if the 6th bit of USISR (Counter Overflow Interrupt) is a 1
                        recieve_data();
                        byte++;
                    }
                }
            }
            USISR |= (1 << 5); // Clear the stop condition flag (USIPF)
            _delay_ms(1000);
            if (data_recieved) {
                led_blink(STATUS_LED, 2);
                data_recieved = 0;
            }
            byte = 0;
        }
        if ((PINB & (1 << ACC_OPTO)) == (1 << ACC_OPTO)) { // If ACC is pulled high (car shut off)
            int time_count = 0;
            PORTB |= (1 << STATUS_LED);   // Turn on the LED to indicate an impending shutdown
            while ((PINB & (1 << ACC_OPTO)) == (1 << ACC_OPTO)) { // Keep checking that the car doesn't turn back on, quit if it does
                _delay_ms(1000);
                time_count++;
                //PORTB ^= (1 << STATUS_LED);
                if (time_count == timeout) {
                    PORTB &= ~(1 << FET_CTL);  // Kill circuit power by toggling the FET pin
                }
            }
            PORTB &= ~(1 << STATUS_LED);  // Turn off the LED, shutdown is canceled.
        }
    }
}