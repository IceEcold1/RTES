#include <stdio.h>
#include "RobotManager.h"
#include "CM730_Serial.h"

/*
	this->arm_manager
	this->leg_manager 
	this->sensor_manager
*/
int main(void)
{
    RobotManager manager;
    manager.init_system();
    manager.start_system();

    manager.leg_manager->move_right_leg_forward();
    manager.arm_manager->move_left_arm_forward();

    manager.sensor_manager->get_gyro_data();
    manager.sensor_manager->get_accelero_data();
    while(1) { }
    return 0;
}