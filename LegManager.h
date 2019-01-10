#ifndef LEG_MANAGER_H
#define LEG_MANAGER_H

#include "SynchronisationServer.h"

class LegManager {
private:
	SynchronisationServer sync_server;
public:
	LegManager(SynchronisationServer sync_server);
	bool move_left_leg();
	bool move_right_leg();
};

#endif
