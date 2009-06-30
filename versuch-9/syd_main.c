#include "stdarg.h"
#include "stdlib.h"
#include "string.h"
#include "asuro.h"

#include "syd/syd.h"
#include "syd/syd_util.h"
#include "syd/syd_timer.h"

#define M_SLOW 60
#define M_FAST 120

char speeds[2];

void sense_light() {
    uint ld[2];
    LineData(ld);
    short diff = ld[0]-ld[1];
    char sgn = (diff>0)?1:-1;
    syd_send_data("diff", 'h', 1, sizeof(short), &diff);
    syd_send_data("line_data", 'h', 2, sizeof(uint), ld);
    if (abs(diff)>15) {
        speeds[0] = -M_FAST * sgn;
        speeds[1] = +M_FAST * sgn;
    } else {
        speeds[0] = M_SLOW;
        speeds[1] = M_SLOW;
    }
}

void sense_buttons() {
    uchar buttons = PollSwitch();
    if (buttons>0) {
        speeds[0] = -M_FAST;
        speeds[1] = -M_FAST;
    }
}

void sense() {
    sense_light();
    sense_buttons();
}

void behave() {
    SetMotorPower(speeds[0], speeds[1]);
}



int main(void) {
    Init();
    syd_message("Lightseeker");

    syd_timer_add(sense, 100, 0);
    syd_timer_add(behave, 100, 0);

    syd_timer_mainloop();
    
    return 0;
}
