#include "ArmManager.h"

bool success;
int offset;
int ADD_TO_SERVO_ANGLE;
int CHANGE_TO_SERVO_ANGLE;
SynchronisationServer SS;
Servo leg_servo;

ArmManager::ArmManager(SynchronisationServer SyncServer)
{
	this.offset = 0;
	this.ADD_TO_SERVO_ANGLE = 99999999; //WIP
	this.CHANGE_TO_SERVO_ANGLE = 99999999; //WIP
	this.SS = SyncServer;
	this.leg_servo = new Servo(SyncServer);
}

/*
 * This function gives all the servo's in the arm (either left or right) the command to move it to a postion,
 * or with the relative it moves these servos to a relative position.
 * Starting with the servo the closest to the body and working outwards.
 * In the event the Servo.move_servo function returns false the remaining servos aren't commanded and the fucntion
 * returns false.
 */
bool move_arm(bool relative, bool left_arm, int servoA, int servoB, int servoC)
{
	success = true;
	if(relative)
	{mode = ADD_TO_SERVO_ANGLE;}
	else
	{mode = CHANGE_TO_SERVO_ANGLE;}
	if(left_arm)
	{
		offset = 1
	}

	success = this.Servo.move_servo(1+offset, mode, servoA)
	if(success)
	{
		success = this.Servo.move_servo(3+offset, mode, servoB)
	}

	if(success)
	{
		success = this.Servo.move_servo(5+offset, mode, servoC)
	}
	return success;
}
