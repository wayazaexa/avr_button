#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include "uart.h"
#include "millis.h"

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
    millis_init(); // Initiera millis
    sei(); // Enable interrupts - behövs för millis

    BIT_SET(DDRB, LED_PIN); // Output mode

    // Sätt knapp-pinnen till input pull-up - hålls hög när knappen inte är tryckt - blir låg när knappen trycks
    BIT_CLEAR(DDRB, BUTTON_PIN); // Input mode
    BIT_SET(PORTB, BUTTON_PIN); // Set to input pull-up

    bool blinking = false;
    millis_t millis_since_last_change = 0;
    millis_t current_millis = 0;
    millis_t delta = 0;
    int counter = 0;

    while (1) {
        current_millis = millis_get();

        if (BUTTON_IS_CLICKED(PINB, BUTTON_PIN)) {
            _delay_ms(70); // Vi börjar med en delay - bättre, men inte helt bra (blir bättre ju längre vi väntar dock)
            printf("%d\n", counter++);
            blinking = !blinking;

            if (!blinking) {
                BIT_CLEAR(PORTB, LED_PIN);
            }
        }
        
        delta = current_millis - millis_since_last_change;

        if (blinking && (delta >= 250)) {
            BIT_FLIP(PORTB, LED_PIN);
            millis_since_last_change = current_millis;
        }
    }
    return 0;
}
