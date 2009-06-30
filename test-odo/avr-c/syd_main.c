#include "stdarg.h"
#include "stdlib.h"
#include "string.h"
#include "asuro.h"

#include "syd.h"
#include "syd_util.h"
#include "syd_calibrate_odometry.h"

int main(void) {

    Init();
    GREEN_LED_ON;
    
    // MotorDir(FWD,FWD);
    // MotorSpeed(128, 128);

    ser_printf("\nSTART\nbuild: %s\n", build());
    
    calibrate_odometry();
    SetMotorPower(0,0);
    
    return 0;
}


