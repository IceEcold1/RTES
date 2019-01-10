#include "ArmManager.h"

ArmManager::ArmManager(SynchronisationServer sync_server)
{
	this->sync_server = sync_server;
}

bool ArmManager::move_left_arm_forward()
{
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