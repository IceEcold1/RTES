#ifndef LEG_MANAGER_H
#define LEG_MANAGER_H

class LegManager {
public:
	bool move_left_leg();
	bool move_right_leg();
	SynchronisationServer sync_server;
};

#endif
