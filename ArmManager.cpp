#include "ArmManager.h"

ArmManager::ArmManager(SynchronisationServer *sync_server)
{
	this->sync_server = sync_server;
}

bool ArmManager::move_left_arm_forward()
{
	manager_command cmd;

	cmd.identifier = (manager_id)arm_manager;
	cmd.action = "move_left_arm.forward";
	
	this->sync_server->give_action(cmd);
	printf("ArmManager::move_left_arm_forward()\n");
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