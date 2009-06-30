#include "stdarg.h"
#include "stdlib.h"
#include "string.h"
#include "asuro.h"

#include "syd/syd.h"
#include "syd/syd_util.h"
#include "syd/syd_timer.h"

#define M_SLOW 60
#define M_FAST 120

enum {
    RESET,
    CALIBRATE,
    COUNT
};

enum {
    SURFACE_BLACK,
    SURFACE_WHITE
};

uint8_t mode = CALIBRATE;

uint8_t surface = SURFACE_WHITE;

uint8_t surface_changes;
uint8_t black_stripes;

uint16_t lds_min = -1;
uint16_t lds_max = 0;

uint8_t switch_old;

uint8_t send_lds_data = 1;
uint8_t send_sf_counter = 1;

void reset_sf_counter() {
    surface = SURFACE_WHITE;
    surface_changes = 0;
    black_stripes = 0;
    dump_counters();
}

void reset_calibration() {
    lds_min = -1;
    lds_max = 0;
}

void on_switch(char index, char state) {
    if (state==1) {
        if (index==0) {
            reset_sf_counter();
        } else if (index==1) {
            reset_calibration();
        } else if (index==2) {
            send_lds_data = !send_lds_data;
        }
    }
}

void check_buttons() {
    char i;
    uint8_t switch_new = PollSwitch();
    for (i=0; i<6; i++) {
        char b_new = (switch_new & (1<<i));
        char b_old = (switch_old & (1<<i));
        if (b_new != b_old) {
            on_switch(i, b_new>0);
        }
    }
    switch_old = switch_new;
}

void calibrate() {
    // SetMotorPower(M_SLOW, M_SLOW);

    uint linedata[2];
    LineData(linedata);
    uint16_t lds = linedata[0] + linedata[1];

    if (lds<lds_min) {
        lds_min = lds;
    } else if (lds>lds_max) {
        lds_max = lds;
    }

    if (send_lds_data) {
        syd_send_data("line_data", 'h', 2, sizeof(uint), linedata);
        syd_send_data("lds", 'h', 1, sizeof(uint16_t), &lds);
        syd_send_data("lds_min", 'h', 1, sizeof(uint16_t), &lds_min);
        syd_send_data("lds_max", 'h', 1, sizeof(uint16_t), &lds_max);
    }
}

void count() {
    uint linedata[2];
    LineData(linedata);
    uint16_t lds = linedata[0] + linedata[1];

    uint8_t surface_new;

    if (lds<(uint16_t)(lds_min*1.25)) {
        surface_new = SURFACE_BLACK;
    } else if (lds>(uint16_t)(lds_max*0.75)) {
        surface_new = SURFACE_WHITE;
    }

    if (surface != surface_new) {
        surface_changes++;
        if (surface_new==SURFACE_BLACK) {
            black_stripes++;
        }
        surface = surface_new;
        dump_counters();
    }
}

void dump_counters() {
    syd_send_data("black_stripes", 'b', 1, sizeof(uint8_t), &black_stripes);
    syd_send_data("surface_changes", 'b', 1, sizeof(uint8_t), &surface_changes);
}


int main(void) {
    Init();
    syd_message("Line Counter");


    FrontLED(ON);

    while (1) {
        check_buttons();
        calibrate();
        count();
    }
    
    return 0;
}
