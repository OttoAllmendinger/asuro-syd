#include "asuro.h"
// #include "encoder_low.h"

#include "syd/syd.h"
#include "syd/syd_util.h"

#include "syd_odo.h"

#define SAMPLES 8

uint samples_r[SAMPLES];
uint samples_l[SAMPLES];

uint avg[2]; // min, max, avg

char pointer=0;
char has_data=0;

char syd_odo_send_data = 0;

uint smp_average(uint* smp_source) {
    char i;
    uint sum=0;

    for (i=0; i<SAMPLES; i++) {
        sum += smp_source[i];
    }

    return (uint)(sum/(double)SAMPLES);
}

void syd_odo_init() {
    int i;
    IsrEnc();
}

void syd_odo_get() {
    uint odo_data[2];
    OdometrieData(odo_data);

    if (pointer>SAMPLES-1) {
        pointer=0;
        has_data=1;
    }

    if (has_data && (pointer%4==0)) {
        avg[0] = smp_average(samples_l);
        avg[1] = smp_average(samples_r);
        if (syd_odo_send_data) {
            syd_send_data("avg", 'h', 2, sizeof(uint), (uchar*) avg);
        }
    } 

    samples_l[pointer] = odo_data[0];
    samples_r[pointer] = odo_data[1];

    pointer++;

    if (syd_odo_send_data) {
        syd_send_data("odo_data", 'h', 2, sizeof(uint), (uchar*)odo_data);
    }
}


uint syd_odo_avg(char idx) {
    if (has_data>0) {
        return avg[idx];
    } else {
        return 1000; // why not
    }
}

