
#include "asuro.h"

#include "syd/syd.h"
#include "syd/syd_util.h"


void main() {
    Init();
    unsigned int od_data[2];
    while (1) {
        OdometryData(od_data);
        ser_printf("od_data=%u,%u\n", od_data[0], od_data[1]);
    }
}
