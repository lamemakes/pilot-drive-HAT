#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SLAVE_ADDR 0x20


int byte = 0;
int read_data; // Used when data is address to slave

int val_in; // Value recieved from master

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
    USICR = (1 << USIWM1) | (1 << USICS0) | (1 << USICS1);  // Set USI interface to I2C, and clock to external w negative edge sampling.
    DDRB = (1 << 4) | (1 << 3) | (1 << 1); //SDA & SCL direction as input, w LED pins as output
    PORTB = 0;
}

void send_ack() {
    USISR |= (1 << USIOIF);
    DDRB |= (1 << 0);  // Initialize the SDA Data Direction Bit
    int clkMask = (1 << USICNT3) | (1 << USICNT2) | (1 << USICNT1) | (1 << USICNT0);
    if (!byte) {    // If this is the first byte...
        //USISR &= clkMask;
        while ((USISR & clkMask) <= (1 << USICNT2)) { // While the counter is <= 4, BUT greater than 2. An extra clock cycle is missed with the address bits, not sure why? Maybe start cond?
            if (USISR & clkMask >= (1 << USICNT1)){
                PORTB |= (1 << 0); // Drive SDA
            }
        }
    } else if (byte > 0) {
        while ((USISR & clkMask) == (1 << 0)) { // While the counter is < 1
            PORTB |= (1 << 0); // Drive SDA
        }
    }
    PORTB &= ~(1 << 0); // Stop SDA drive & change data direction
    DDRB &= ~(1 << 0);
    USISR = 0; // Reset counter
}

void recieve_data(){
    int dataIn = USIBR / 2; // It appears the external clock is sampled on BOTH (pos/neg) edges, thus gets double the intended value?
    if (!byte && (dataIn == SLAVE_ADDR)){ // If this is the first byte & the buffer == slave address...
        PORTB |= (1 << 1);  // Turn on LED
        read_data = 1;  // Set data read to true.
        send_ack();
    } else if (byte){
        val_in = dataIn;
        send_ack();
        read_data = 0;
    }

}

int main (void){

    initialize();

    while(1){
        PORTB = (1 << 4);
        val_in = 0;
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
            PORTB ^= (1 << 4);
            if (val_in) {
                led_blink(3, val_in);
            }
            byte = 0;
        }
        PORTB &= ~(1 << 1);
    }
}