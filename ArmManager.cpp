#include "ArmManager.h"

ArmManager::ArmManager(SynchronisationServer *sync_server)
{
	this->command_function = new CommandFunction(sync_server, (manager_id)arm_manager);
}

bool ArmManager::move_left_arm_forward()
{
	this->command_function->resolve_command("move_left_arm.forward");
	this->command_function->resolve_command("s2.rotate_servo.1300");
	this->command_function->resolve_command("s6.rotate_servo.2100");
	return true;
}

bool ArmManager::move_left_arm_backward()
{
	return true;
}

bool ArmManager::move_right_arm_forward()
{
	return true;
}

bool ArmManager::move_right_arm_backward()
{
	return true;
}