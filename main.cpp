#include <stdio.h>
#include "RobotManager.h"
#include "CM730_Serial.h"

int main(void)
{
    RobotManager manager;
    manager.init_system();
    manager.start_system();

    while(1) { }
    return 0;
}