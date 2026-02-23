#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include "uart.h"

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

unsigned char buttonTickResult_PB1 = 0;

typedef enum{
    Click_status_Pressed,
    Click_status_Released,
    Click_status_Not_sure,
} Click_status;

Click_status clicked(unsigned char tick_vector) {
    // Create a mask with the 5 lower bits set to 1 (0b00011111 = 31)
    unsigned char mask = 31;

    //00011111 5 senaste
    if ((tick_vector & mask) == mask) {
        return Click_status_Pressed; // 5 senaste värden är höga
    }
    else if ((tick_vector & mask) == 0) {
        return Click_status_Released; // 5 senaste värden är låga
    }
    else {
        return Click_status_Not_sure;
    }
}

ISR(TIMER2_OVF_vect)
{
    TCNT2 = 5; // Timer Preloading
    bool sample = BUTTON_IS_CLICKED(PINB, BUTTON_PIN);

    // 1011111  -> 8 st millisekunder
    buttonTickResult_PB1 = (buttonTickResult_PB1 << 1) | sample;
}

void timer2_init()
{
  TCCR2A = 0;   // Init Timer2A
  TCCR2B = 0;   // Init Timer2B
  TCCR2B |= 7;  // Prescaler = 1024
  TCNT2 = 5;    // Timer Preloading
  TIMSK2 |= 1;  // Enable Timer Overflow Interrupt
}

int main(void) {
    init_serial();
    timer2_init();
    sei();

    BIT_SET(DDRB, LED_PIN);

    //Sätt till INPUT_PULLUP
    BIT_CLEAR(DDRB, BUTTON_PIN);
    BIT_SET(PORTB, BUTTON_PIN);

    bool isOn = false;
    bool canRelease = false;
    int changes = 0;
    
    while (1) {
        Click_status status = clicked(buttonTickResult_PB1);

        if (status == Click_status_Pressed) {
            canRelease = true;
        }

        if (canRelease && status == Click_status_Released) {
            isOn = !isOn;
            printf("%d\n", changes++);
            canRelease = false;
        }

        if (isOn) {
            BIT_SET(PORTB, LED_PIN);
        }
        else {
            BIT_CLEAR(PORTB, LED_PIN);
        }
    }
    return 0;
}
