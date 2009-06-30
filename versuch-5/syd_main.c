#include "asuro.h"
#include "avr/io.h"
#include "avr/interrupt.h"

#define SAMPLES 128


uint8_t volatile count;

ISR(TIMER0_OVF_vect) {
    count++;

    if (count==61) {
        RED_LED_ON;
        count=0;
    }
}

void main() {
    Init();

    TCCR0 |= (1<<CS02)|(CS01);
    TIMSK |= (1<<TOIE0);
    TCNT0 = 0;
    count = 0;
    DDRC |= 0x0F;
    sei();

    while (1);
}
