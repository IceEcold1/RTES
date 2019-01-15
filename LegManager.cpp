#include "LegManager.h"

LegManager::LegManager(SynchronisationServer *sync_server)
{
	this->command_function = new CommandFunction(sync_server, (manager_id)arm_manager);
}

bool LegManager::move_left_leg_forward()
{
	this->command_function->resolve_command("move_left_leg.forward");
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