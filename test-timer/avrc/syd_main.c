#include "asuro.h"
#include "avr/io.h"
#include "avr/interrupt.h"

#include "syd_util.h"

#define MAX_CB 8

typedef struct {
    int interval;
    int counter;
    void (*function)(void);
} s_callback;

s_callback callbacks[MAX_CB];

int n_cb = 0;

volatile char overflow;

ISR(TIMER0_OVF_vect) {
    overflow = 1;
}


void add_callback(int interval, int counter, void (*fct)(void)) {
    if (n_cb<MAX_CB) {
        callbacks[n_cb].interval = interval;
        callbacks[n_cb].counter = counter;
        callbacks[n_cb].function = fct;
        ser_printf("added callbacks[%d]{interval=%d, counter=%d}\n", 
                n_cb, callbacks[n_cb].interval, callbacks[n_cb].counter);
        n_cb++;
    }
}

void dump_callbacks() {
    int i; 
    for (i=0; i<n_cb; i++) {
        ser_printf("callbacks[%d].interval=%d .counter=%d\n",
                i, callbacks[i].interval, callbacks[i].counter);
    }
}


void led_1() {
    GREEN_LED_ON;
}

void led_2() {
    GREEN_LED_OFF;
}



int main() {
    Init();
    init_util();
    // Init

    // Initialize Timer
    TCCR0 |= (1<<CS02)|(CS01);
    TIMSK |= (1<<TOIE0);
    TCNT0 = 0;
    DDRC |= 0x0F;
    sei();

    GREEN_LED_OFF;

    add_callback(4, 0, led_1);
    add_callback(4, 2, led_2);

    dump_callbacks();

    int cnt=0;
    while (1) {
        if (overflow) {
            int i;
            for (i=0; i<n_cb; i++) {
                s_callback *cb = &callbacks[i];
                if (cb->counter==cb->interval) {
                    cb->function();
                    cb->counter = 0;
                } else {
                    cb->counter++;
                }
            }
            overflow = 0;
            cnt = 0;
        } else {
            cnt++;
        }
    }

    return 0;
}
