#ifndef LEG_MANAGER_H
#define LEG_MANAGER_H

class LegManager {
public:
	bool move_leg(bool relative, bool left_leg, int servoA, int servoB, int servoC, int servoD, int servoE, int servoF);
};

#endif
