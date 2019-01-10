#include "LegManager.h"


LegManager::LegManager(SynchronisationServer sync_server)
{
	this->sync_server = sync_server;
}

bool LegManager::move_left_leg()
{
	return true;
}

bool LegManager::move_right_leg()
{
	return true;
}