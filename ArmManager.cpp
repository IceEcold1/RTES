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
	cmd.resolved = false;
	
	this->sync_server->give_action(cmd);

	while(!cmd.resolved)
		cmd = this->sync_server->get_result(cmd);
	if(cmd.successful)
		printf("ArmManager::move_left_arm_forward(), successfull\n");
	else
		printf("ArmManager::move_left_arm_forward(), NOT successfull\n");
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