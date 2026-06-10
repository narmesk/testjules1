#include "mcc_generated_files/system.h"
#include "mcc_generated_files/timer5.h"
#include "asda2_control.h"

int main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    // Initialize Servo structures and CANopen state
    ASDA2_Initialize();

    // Start CANopen nodes (Global NMT Start)
    CANopen_Start_Nodes();

    // Initialize Master Timer (4ms)
    Timer5_Initialize();

    while (1)
    {
        // Placeholder for PC Application communication parser
        // Example: If G01 command received, call move_to_absolute_position(...)

        /*
        float example_target[4] = {100.5f, 200.0f, 0.0f, 50.0f};
        if (!plan_check_full_buffer()) {
            move_to_absolute_position(example_target, 1200.0f); // 1200 mm/min
        }
        */

        // Background tasks and housekeeping
    }

    return 0;
}
