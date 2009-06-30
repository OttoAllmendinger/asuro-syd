#include "asuro.h"

#include "syd.h"
#include "syd_util.h"
#include "syd_calibrate_odometry.h"

#define N_SAMPLES 64



void record_send() {
    // send_var("speed", 'b', sizeof(char), &speed);

    ushort samples_r[N_SAMPLES];
    ushort samples_l[N_SAMPLES];
    uint od_data[2];

    int i;
    for (i=0; i<N_SAMPLES; i++) {
        OdometrieData(od_data);
        samples_l[i] = od_data[0];
        samples_r[i] = od_data[1];
    }

    send_data("samples_l", 'h', N_SAMPLES, sizeof(ushort), (void*) samples_l);
    send_data("samples_r", 'h', N_SAMPLES, sizeof(ushort), (void*) samples_r);
}

void calibrate_odometry() {
    char speed=0;
    while (speed<127) {
        int i;
        send_data("speed", 'c', 1, sizeof(char), &speed);
        for (i=0; i<8; i++) {
            SetMotorPower(speed, speed);
            record_send();
        }
        speed += 16;
    }
}


/*
ulong current_time = Gettime();
ulong time_delta = (current_time-last_output);

if (time_delta>2000) {
    ser_printf("(cur/min/max) left: %4d/%4d/%4d    right: %d/%d/%d\n\r",
            odo_l, odo_l_min, odo_l_max, odo_r, odo_r_min, odo_r_max);
    last_output = current_time;
}
*/

