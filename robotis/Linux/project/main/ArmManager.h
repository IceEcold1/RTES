#ifndef ARM_MANAGER_H
#define ARM_MANAGER_H

class ArmManager {
public:
	bool move_Arm(bool relative, bool left_arm, int servoA, int servoB, int servoC);
};

#endif
