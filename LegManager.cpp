#include "LegManager.h"

LegManager::LegManager(SynchronisationServer *sync_server)
{
	this->sync_server = sync_server;
}

bool LegManager::move_left_leg_forward()
{
	manager_command cmd;

	cmd.identifier = (manager_id)leg_manager;
	cmd.action = "move_left_leg.forward";
	
	this->sync_server->give_action(cmd);
	printf("ArmManager::move_left_leg_forward()\n");
	return true;
}

bool LegManager::move_left_leg_backward()
{
	return true;
}

bool LegManager::move_right_leg_forward()
{
	return true;
}

bool LegManager::move_right_leg_backward()
{
	return true;
}