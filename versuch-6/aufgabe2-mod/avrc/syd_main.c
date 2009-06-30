#include "stdarg.h"
#include "stdlib.h"
#include "string.h"
#include "asuro.h"

#include "syd/syd.h"
#include "syd/syd_util.h"
#include "syd/syd_timer.h"

#include "syd_lc.h"



char is_driving = 0;

void check_buttons() {
    uchar buttons = PollSwitch();
    if (buttons) {
        ser_printf("button:%02X\n");
    }
    if (buttons==0x01) {
        if (is_driving) {
            SetMotorPower(0, 0);
            is_driving=0;
        } else {
            SetMotorPower(100, -100);
            is_driving=1;
        }
    }
}

int main(void) {
    Init();
    syd_message();

    syd_timer_add(check_buttons, 100, 0);
    syd_timer_add(syd_lc_get, 100, 0);

    syd_timer_mainloop();
    
    return 0;
}


