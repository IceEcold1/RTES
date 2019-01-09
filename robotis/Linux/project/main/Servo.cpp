#include "Servo.h"

SynchronisationServer SS;

Servo::Servo(SynchronisationServer SyncServer)
{
	this.SS SyncServer;
}

bool move_servo(Servo_n, aspect, value)
{
	return SS.moveservo(Servo_n, aspect, value); //returns true is successful and false when it fails //WIP
}
