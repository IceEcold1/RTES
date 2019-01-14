#include <stdio.h>
#include "RobotManager.h"
#include "CM730_Serial.h"

int main(void)
{
    RobotManager manager;
    manager.init_system();
    manager.start_system();
    manager.leg_manager->move_left_leg_forward();
    while(1) { }
    return 0;
}