#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart.h"

#define BAUD 9600 // Needed by uart.h

// B (digital pin 8 to 13)
// C (analog input pins)
// D (digital pins 0 to 7)
#define LED_PIN 2
#define BUTTON_PIN 1

#define BIT_SET(a, b) (a |= (1U << b))
#define BIT_CLEAR(a, b) (a &= ~(1U << b))
#define BIT_FLIP(a, b) (a ^= (1U << b))
#define BIT_CHECK(a, b) (a & (1U << b)) 


#define BUTTON_IS_CLICKED(PINB, BUTTON_PIN) !BIT_CHECK(PINB, BUTTON_PIN)

int main(void){
    init_serial(); // Initiera uart

    // Sätt knapp-pinnen till input pull-up - hålls hög när knappen inte är tryckt - blir låg när knappen trycks
    BIT_CLEAR(DDRB, BUTTON_PIN); // Input mode
    BIT_SET(PORTB, BUTTON_PIN); // Set to input pull-up

    int counter = 0;

    while (1) {
        if (BUTTON_IS_CLICKED(PINB, BUTTON_PIN)) {
            printf("%d\n", counter++);
        }
    }
    return 0;
}
