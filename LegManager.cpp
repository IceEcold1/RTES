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
	cmd.resolved = false;

	this->sync_server->give_action(cmd);

	while(!cmd.resolved)
		cmd = this->sync_server->get_result(cmd);
	if(cmd.successful)
		printf("LegManager::move_left_arm_forward(), successfull\n");
	else
		printf("LegManager::move_left_arm_forward(), NOT successfull\n");
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