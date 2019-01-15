#ifndef ARM_MANAGER_H
#define ARM_MANAGER_H

#include "SynchronisationServer.h"
#include "CommandFunction.h"

class ArmManager {
private:
	SynchronisationServer *sync_server;
	CommandFunction *command_function;
public:
	ArmManager(SynchronisationServer *sync_server);
	bool move_left_arm_forward();
	bool move_left_arm_backward();
	bool move_right_arm_forward();
	bool move_right_arm_backward();
};

#endif
