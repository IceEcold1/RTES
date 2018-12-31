#include "RobotManager.h"

bool RobotManager::init_system()
{
	SynchronisationServer syncServer;
	vector<struct ltsa_export> data = syncServer.read_ltsa_exports();
	int size = (int)data.size();
	for(int i = 0; i < size; i++)
	{
		//FspProcess process(0, );
		//processes.push_back(process);
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

vector<string> RobotManager::get_alphabet(string data)
{
	for(int i = 0; data[i] != '\0'; i++)
	{
		cmatch results;
		regex rx("(?<=,)([^,\n]+)(?=,)");
		regex_search(data.c_str(), results, rx);
		for(int i = 0; i < (int)results.size(); i++)
		{
			cout << i << ": '" << results[i].str() << "'\n";
		}
	}
}

vector<string> RobotManager::get_sensitivity_list(int state, string data)
{

}