#include <stdio.h>
#include "RobotManager.h"

void sighandler(int sig)
{
    exit(0);
}

int main(void)
{
    RobotManager manager;
    manager.init_system();

    // Dit moet later omlaag maar kan nu niet omdat de main escaped vanwege ontbreken cm730 in VM
    manager.start_system();
    while(1){}
    return 0;
}