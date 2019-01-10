#ifndef ARM_MANAGER_H
#define ARM_MANAGER_H

#include "SynchronisationServer.h"

class ArmManager {
private:
	SynchronisationServer *sync_server;
	
public:
	ArmManager(SynchronisationServer *sync_server);
	bool move_arm(bool relative, bool left_arm, int servoA, int servoB, int servoC);
};

#endif
