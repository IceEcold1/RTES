#ifndef LEG_MANAGER_H
#define LEG_MANAGER_H

#include "SynchronisationServer.h"

class LegManager {
private:
	SynchronisationServer *sync_server;
public:
	LegManager(SynchronisationServer *sync_server);
	bool move_left_leg_forward();
	bool move_left_leg_backward();
	bool move_right_leg_forward();
	bool move_right_leg_backward();
};

#endif
