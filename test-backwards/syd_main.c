#include "asuro.h"

#include "syd/syd.h"
#include "syd/syd_util.h"
#include "syd/syd_timer.h"

int main(void) {
    Init();

    SetMotorPower(-100, -100);



    char switches;

    while(1) {
        /*
        switches = PollSwitch();
        if (switches & 1) {
            SetMotorPower(100, 100);
        } else if (switches & 2) {
            SetMotorPower(-100, -100);
        } else if (switches & 4) {
            SetMotorPower(0, 0);
        }
        */
    }
    
    return 0;
}


