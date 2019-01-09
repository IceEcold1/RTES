#ifndef ARM_MANAGER_H
#define ARM_MANAGER_H

#include "SynchronisationServer.h"
#include "Servo.h"

class ArmManager {
private:
	bool success;
	int offset;
	int ADD_TO_SERVO_ANGLE;
	int CHANGE_TO_SERVO_ANGLE;
	SynchronisationServer SS;
	Servo leg_servo;
	
public:
	ArmManager(SynchronisationServer SyncServer);
	bool move_Arm(bool relative, bool left_arm, int servoA, int servoB, int servoC);
};

#endif
