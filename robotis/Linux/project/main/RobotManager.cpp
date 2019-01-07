#include "RobotManager.h"

bool RobotManager::init_system()
{
	vector<struct ltsa_export> ltsa_data = this->read_ltsa_exports();
	int size = (int)ltsa_data.size(), init_state = 0;

	for(int i = 0; i < size; i++)
	{
		FspProcess *process = new FspProcess(ltsa_data[i].process_id, init_state, this->get_alphabet(ltsa_data[i].fsp_data), this->get_sensitivity_list(init_state, ltsa_data[i].fsp_data));
		this->processes.push_back(*process);
	}
	this->hds = new HDS(init_state, alphabets, sens_lists);
	this->ss = new SynchronisationServer();
	this->armManager = new ArmManager();
	this->legManager = new LegManager();
	this->sensorManager = new SensorManager();
	return true;
}

/*
 * Starts the threads of the initialized objects
 */
bool RobotManager::start_system()
{
	for(int i = 0; i < (int)this->processes.size(); i++)
	{
		new boost::thread(boost::bind(&FspProcess::run, &this->processes[i]));
	}
	new boost::thread(boost::bind(&HDS::run, this->hds));
	new boost::thread(boost::bind(&SynchronisationServer::run, this->ss));
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

vector<struct ltsa_export> RobotManager::read_ltsa_exports()
{
	vector<struct ltsa_export> ltsa_exports;
	string data, path;
	DIR* dir = opendir("LTSA"); // Open the 'LTSA' directory containing the LTSA exports
	dirent* file;

	while((file = readdir(dir)) != NULL)
	{
		if(file->d_name[0] != '.')
		{
			if(file->d_name)
			{

			}
			struct ltsa_export export_data;
			ifstream ltsa_export;

			sprintf((char*)path.c_str(), "LTSA/%s", file->d_name); // String-format the path of the LTSA export file
			ltsa_export.open(path.c_str()); // Open the file using the generated path
			while(getline(ltsa_export, data)) // Read trough the selected file line by line
			{
				export_data.fsp_data.push_back(data);
			}
			export_data.process_id = file->d_name;
			ltsa_exports.push_back(export_data); // Add the LTSA data to the sting vector
			ltsa_export.close(); // Close the file
		}
	}
	return ltsa_exports;
}