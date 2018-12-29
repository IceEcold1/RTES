#ifndef ROBOT_MANAGER_H
#define ROBOT_MANAGER_H

#include "SynchronisationServer.h"
#include "HDS.h"
#include "ArmManager.h"
#include <stdio.h>

class RobotManager {
public:
	bool init_system();
private:
	bool start_system();
};

#endif
