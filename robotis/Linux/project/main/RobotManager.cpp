#include "RobotManager.h"

bool RobotManager::init_system()
{
	SynchronisationServer syncServer;
	vector<struct ltsa_export> ltsa_data = syncServer.read_ltsa_exports();
	int size = (int)ltsa_data.size(), init_state = 0;

	for(int i = 0; i < size; i++)
	{
		vector<string> alphabet = this->get_alphabet(ltsa_data[i].fsp_data);
		FspProcess *process = new FspProcess(ltsa_data[i].process_id, init_state, alphabet, this->get_sensitivity_list(init_state, ltsa_data[i].fsp_data));
		this->processes.push_back(*process);
	}
	/*HDS hds;
	ArmManager arms;
	LegManager legs;
	SensorManager sensors;*/
	return true;
}

bool RobotManager::start_system()
{
	printf("size%d\n", (int)this->processes.size());
	for(int i = 0; i < (int)this->processes.size(); i++)
	{
		new boost::thread(boost::bind(&FspProcess::run, &this->processes[i]));
	}
	return true;
}

vector<string> RobotManager::get_alphabet(vector<string> data)
{
	POSIX::Regex re;
	POSIX::Match m;
	vector<string> res;
	int size = (int)data.size();

	re.compile("\\(.*?,(.*?),.*?\\)"); // Regex for the second parameter of the data

	for(int i = 1; i < size; i++)
	{
		m = re.match(data[i]);
		if(find(res.begin(), res.end(), m.group(1)) == res.end())
			res.push_back(m.group(1));
	}
	return res;
}

vector<string> RobotManager::get_sensitivity_list(int state, vector<string> data)
{
	POSIX::Regex re;
	POSIX::Match m;
	vector<string> res;
	int size = (int)data.size();

	for(int i = 1; i < size; i++)
	{
		re.compile("\\((.*?),.*?,.*?\\)"); // Regex for the first parameter os the data
		m = re.match(data[i]);
		if(atoi(m.group(1).c_str()) == state)
		{
			re.compile("\\(.*?,(.*?),.*?\\)"); // Regex for the first parameter os the data
			m = re.match(data[i]);
			res.push_back(m.group(1));
		}
	}
	return res;
}