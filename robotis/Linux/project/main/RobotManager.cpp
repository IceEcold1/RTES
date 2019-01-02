#include "RobotManager.h"

bool RobotManager::init_system()
{
	SynchronisationServer syncServer;
	vector<struct ltsa_export> data = syncServer.read_ltsa_exports();
	int size = (int)data.size();
	for(int i = 0; i < size; i++)
	{
		this->alphabets = this->get_alphabet(data[i].fspData);
		this->get_sensitivity_list
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

vector<string> RobotManager::get_alphabet(vector<string> data)
{
	POSIX::Regex re;
	POSIX::Match m;
	vector<string> res;
	string temp_res;

	for(int i = 0; i < (int)data.size(); i++)
	{
		re.compile("\\(.*?,(.*?),.*?\\)");
		m = re.match(data[i]);
		temp_res = m.group(1);
		if(i != 0)
		{
			if(find(res.begin(), res.end(), temp_res) == res.end())
				res.push_back(temp_res);
		}
	}
	printf("\n");
	return res;
}

vector<string> RobotManager::get_sensitivity_list(int state, string data)
{

}