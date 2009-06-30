#include "asuro.h"

#include "syd/syd.h"
#include "syd/syd_util.h"

void syd_lc_get() {
    FrontLED(ON);
    uint ld[2];
    LineData(ld);
    ser_printf("linedata: %d %d\n", ld[0], ld[1]);
    // syd_send_data("line_data", 'h', 2, sizeof(uint), *ld);
}
