#include "ArmManager.h"

ArmManager::ArmManager(SynchronisationServer sync_server)
{
	this->sync_server = sync_server;
}

/*
 * This function gives all the servo's in the arm (either left or right) the command to move it to a postion,
 * or with the relative it moves these servos to a relative position.
 * Starting with the servo the closest to the body and working outwards.
 * In the event the Servo.move_servo function returns false the remaining servos aren't commanded and the fucntion
 * returns false.
 */
bool::ArmManager::move_arm(bool relative, bool left_arm, int servoA, int servoB, int servoC)
{
	return true;
}
