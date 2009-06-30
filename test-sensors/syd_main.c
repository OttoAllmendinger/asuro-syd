#include "asuro.h"

#include "syd/syd.h"
#include "syd/syd_util.h"

#define SPEED_STEP 32

#define DEBUG 0x80

#define BATTERY (DEBUG | 1)
#define SENSOR_LINE 0
#define SENSOR_ODOMETRY 1
#define SENSOR_SWITCHES 2

#define SENSOR_CUSTOM 16


#define SURFACE_DARK 0
#define SURFACE_LIGHT 1





uint16_t line_data[2];
uint16_t line_data_avg[2];

uint16_t odo_data[2];

uint8_t surface_changes[2];
uint8_t surface_state[2];

uint16_t surface_switch_time[2];
uint16_t surface_switch_time_delta[2];


char switches;




void read_sensor_data() {
    LineData(line_data);
    OdometryData(odo_data);
    switches = PollSwitch();

    surface_data(0);
    surface_data(1);
}

void send_sensor_data() {
    send(SENSOR_LINE,     line_data, U2, 2);
    send(SENSOR_ODOMETRY, odo_data,  U2, 2);
    send(SENSOR_SWITCHES, &switches, U1, 1);
    send(SENSOR_CUSTOM + 0, surface_state,   U1, 2);
    send(SENSOR_CUSTOM + 1, surface_changes, U1, 2);
    send(SENSOR_CUSTOM + 2, surface_switch_time,    U2, 2);
    // send(SENSOR_CUSTOM + 3, surface_switch_time_delta,    U2, 2);
}

void calibrate_linedata() {
    ulong start_time = Gettime();
    ulong i;
    uint32_t ldsum[2];
    uint ld[2];
    uint tdiff = 0;

    while (tdiff<1000) {
        LineData(ld);
        ldsum[0] += ld[0];
        ldsum[1] += ld[1];
        tdiff = Gettime()-start_time;
        i++;
    }

    line_data_avg[0] = (uint16_t)((double)(ldsum[0])/(double)i);
    line_data_avg[1] = (uint16_t)((double)(ldsum[1])/(double)i);

    send(DEBUG | 2, line_data_avg, U2, 2);
}

void syd_init() {
    FrontLED(ON);
    calibrate_linedata();
}


void surface_data(int i) {
    char sfs;
    if (line_data[i] < (line_data_avg[i]*0.75)) {
        sfs = SURFACE_DARK;
    } else {
        sfs = SURFACE_LIGHT;
    }

    if (sfs != surface_state[i]) {
        surface_changes[i]++;
        surface_switch_time[i] = Gettime();
        surface_state[i] = sfs;
    } 
}

void set_speeds() {
}


int main(void) {
    Init();

    char init[] = "SYD INIT";
    int16_t battery = Battery();
    send(DEBUG,   init,     S1, 8);
    send(BATTERY, &battery, S2, 1);


    syd_init();

    while(1) {
        read_sensor_data();
        send_sensor_data();
        set_speeds();
    }
    
    return 0;
}


