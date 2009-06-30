#include "stdarg.h"
#include "stdlib.h"
#include "string.h"
#include "asuro.h"

#include "syd/syd.h"
#include "syd/syd_util.h"
#include "syd/syd_timer.h"


#include "syd_odo.h"


char is_driving = 0;
char bled = 0;

void check_buttons() {
    uchar buttons = PollSwitch();
    if (buttons==0x01) {
        if (is_driving) {
            SetMotorPower(0, 0);
            is_driving=0;
        } else {
            SetMotorPower(64, -64);
            is_driving=1;
        }
    } else if (buttons==0x02) {
        IsrEnc();
        GoTurn(0, 90, 128);
    } else if (buttons==0x04) {
        syd_odo_send_data = !syd_odo_send_data;
    } else if (buttons==0x08) {
        if (bled) {
            BackLED(OFF, OFF);
            bled=0;
        } else {
            BackLED(ON, ON);
        }
    }
}

int main(void) {
    Init();
    syd_message();

    syd_timer_add(check_buttons, 100, 0);
    syd_timer_add(syd_odo_get, 100, 0);

    syd_timer_mainloop();
    
    return 0;
}


