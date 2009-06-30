#include "asuro.h"
#include "avr/io.h"
#include "avr/interrupt.h"

#include "syd_timer.h"
#include "syd_util.h"

#define MAX_CB 8

#define MS 3.9

volatile char overflow;

ISR(TIMER0_OVF_vect) {
    overflow = 1;
}

typedef struct {
    uint16_t interval;
    uint16_t counter;
    void (*function)(void);
} s_callback;

s_callback callbacks[MAX_CB];

char n_callbacks = 0;

void syd_init_timer() {
    // set prescaler to FCPU/64
    TCCR0 |= (1<<CS01)|(CS00);

    // activate timer
    TIMSK |= (1<<TOIE0);
    TCNT0 = 0;
    sei();
}


void syd_timer_add(void (*fct)(void), uint16_t ms, uint16_t counter) {

    if (n_callbacks<MAX_CB) {
        callbacks[n_callbacks].interval = (uint16_t)(ms * MS);
        callbacks[n_callbacks].counter = counter;
        callbacks[n_callbacks].function = fct;
        n_callbacks++;
    }
}

void syd_timer_mainloop() {
    syd_init_timer();
    while (1) {
        if (overflow) {
            char i;
            for (i=0; i<n_callbacks; i++) {
                s_callback *cb = &callbacks[i];
                if (cb->counter==cb->interval) {
                    cb->function();
                    cb->counter = 0;
                } else {
                    cb->counter++;
                }
            }
            overflow = 0;
        }
    }
}
