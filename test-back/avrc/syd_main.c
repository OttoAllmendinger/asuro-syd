#include "stdarg.h"
#include "stdlib.h"
#include "string.h"
#include "asuro.h"

// #include "math.h"

#include "syd/syd.h"
#include "syd/syd_util.h"
#include "syd/syd_timer.h"

int main(void) {
    Init();
    syd_message("Backward Testing");

    SetMotorPower(-100, -100);

    while(1) {
    }
    
    return 0;
}


