#include "asuro.h"

#include "syd/syd.h"
#include "syd/syd_util.h"

// gerade MotorSpeed(140, 150);
//
// line_data[0] = links
// line_data[1] = rechts

uint ldavg[2];
uint line_data[2];
int lr_diff=0;
int lr_diff_a=0;

char lrswitch=1;

void calibrate_linedata(uint* avg) {
    FrontLED(ON);
    uint32_t ldsum[2] = {0,0};
    const int samples = 255;
    int i;

    for (i=0; i<samples; i++) {
        uint line_data[2];
        LineData(line_data);
        ldsum[0] += line_data[0];
        ldsum[1] += line_data[1];
    }

    avg[0] = (ldsum[0]/samples);
    avg[1] = (ldsum[1]/samples);
}

void read_linedata() {
    int ld_norm[2];

    LineData(line_data);

    ld_norm[0] = (line_data[0]-ldavg[0]);
    ld_norm[1] = (line_data[1]-ldavg[1]);

    lr_diff = (ld_norm[0] - ld_norm[1]);
    lr_diff_a = (lr_diff_a*3 + lr_diff*2)/5;

    send(0, line_data,  S2, 2);
    send(1, ld_norm,    S2, 2);
    send(2, &lr_diff,   S2, 1);
    send(3, &lr_diff_a, S2, 1);
}

void read_switches() {
    char switches = PollSwitch();
    if (switches&1) {
        lrswitch=1;
    } else if (switches&2) {
        lrswitch=-1;
    }
}

void set_speeds() {
    int speeds[2];
    const limit = 10;
    if (lr_diff<-limit) {
        speeds[0] = 0;
        speeds[1] = 80;
    } else if (lr_diff>limit) {
        speeds[0] = 80;
        speeds[1] = 0;
    } else {
        speeds[0] = 60;
        speeds[1] = 60;
    }

    SetMotorPower(speeds[0], speeds[1]);
}


int main(void) {
    Init();
    // syd_message("competition");

    char switches = 0;

    while (!switches) {
        calibrate_linedata(ldavg);
        send(32+0, ldavg, U2, 2);
        switches = PollSwitch();
    }

    while(1) {
        read_linedata();
        read_switches();
        set_speeds();
    }
    return 0;
}


