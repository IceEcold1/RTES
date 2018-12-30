#include "RobotManager.h"

bool RobotManager::init_system()
{
	SynchronisationServer syncServer;
	vector<string> data = syncServer.read_ltsa_exports();
	for(int i = 0; i < (int)data.size(); i++)
	{
		printf("ltsa_exports[%d]: \n%s\n", i, data[i].c_str());
	}
	/*HDS hds;
	ArmManager arms;
	LegManager legs;
	SensorManager sensors;*/
	return true;
}

bool RobotManager::start_system()
{
	return true;
}