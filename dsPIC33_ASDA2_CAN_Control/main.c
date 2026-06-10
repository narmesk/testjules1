#include "mcc_generated_files/system.h"
#include "asda2_control.h"

int main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    while (1)
    {
        // ASDA-A2 Servo Control State Machine
        ASDA2_DriveStateMachine();

        // Background tasks
    }

    return 1;
}
