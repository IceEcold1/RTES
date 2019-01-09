#include "LegManager.h"
#include "servo.h"

bool success;
int offset;
int ADD_TO_SERVO_ANGLE;
int CHANGE_TO_SERVO_ANGLE;
SynchronisationServer SS;
Servo leg_servo;

LegManager::LegManager(SynchronisationServer SyncServer)
{
	this.offset = 0;
	this.ADD_TO_SERVO_ANGLE = 99999999; //WIP
	this.CHANGE_TO_SERVO_ANGLE = 99999999; //WIP
	this.SS = SyncServer;
	this.leg_servo = new Servo(SyncServer);
}

/*
 * This function gives all the servo's in the Leg (either left or right) the command to move it to a position,
 * or with the relative it moves these servos to a relative position.
 * Starting with the servo the closest to the body and working outwards.
 * In the event the Servo.move_servo function returns false the remaining servos aren't commanded and the function
 * returns false.
 */
bool move_leg(bool relative, bool left_leg, int servoA, int servoB, int servoC, int servoD, int servoE, int servoF)
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

	success = this.Servo.move_servo(7+offset, mode, servoA)
	if(success)
	{
		success = this.Servo.move_servo(9+offset, mode, servoB)
	}
		if(success)
	{
		success = this.Servo.move_servo(11+offset, mode, servoC)
	}
		if(success)
	{
		success = this.Servo.move_servo(13+offset, mode, servoD)
	}
		if(success)
	{
			success = this.Servo.move_servo(15+offset, mode, servoE)
	}
	if(success)
	{
		success = this.Servo.move_servo(17+offset, mode, servoF)
	}
	return success;
}
