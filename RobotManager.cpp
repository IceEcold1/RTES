#include "RobotManager.h"

bool RobotManager::init_system()
{
	vector<struct ltsa_export> ltsa_data = this->read_ltsa_exports();
	int size = (int)ltsa_data.size(), init_state = 0;
	printf("RobotManager::Initializing systems\n");
	/*HDS constructor must create base class FspProcess first, since HDS has no sens list, send an empty vector*/
	this->hds = new HDS(this->hds_data.process_id, init_state, this->get_alphabet(this->hds_data.fsp_data), this->hds_data.fsp_data);
	this->sync_server = new SynchronisationServer(this->hds);
	this->sync_server->add_process(this->hds);
	for(int i = 0; i < size; i++)
	{
		FspProcess *process = new FspProcess(ltsa_data[i].process_id, init_state, this->get_alphabet(ltsa_data[i].fsp_data), ltsa_data[i].fsp_data, this->sync_server);
		this->processes.push_back(process);
		this->sync_server->add_process(process);
	}
	return true;
}

/*
 * Starts the threads of the initialized objects
 */
bool RobotManager::start_system()
{
	printf("RobotManager::Starting threads.\n");
	for(int i = 0; i < (int)this->processes.size(); i++)
	{
		new boost::thread(boost::bind(&FspProcess::run, this->processes[i]));
		while(this->processes[i]->get_started_bool() != true) {};
	}
	usleep(100);
	new boost::thread(boost::bind(&HDS::run, this->hds));
	new boost::thread(boost::bind(&SynchronisationServer::run, this->sync_server));
	printf("RobotManager::Threads succesfully started.\n");
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
			struct ltsa_export export_data;
			ifstream ltsa_export;

			sprintf((char*)path.c_str(), "LTSA/%s", file->d_name); // String-format the path of the LTSA export file
			ltsa_export.open(path.c_str()); // Open the file using the generated path
			string fileName(file->d_name);
			if(fileName.find("HDS") == 0)
			{
				while(getline(ltsa_export, data)) // Read trough the selected file line by line
				{
					this->hds_data.fsp_data.push_back(data);
				}
				this->hds_data.process_id = file->d_name;
			}
			else
			{
				while(getline(ltsa_export, data)) // Read trough the selected file line by line
				{
					export_data.fsp_data.push_back(data);
				}
				export_data.process_id = file->d_name;
				ltsa_exports.push_back(export_data); // Add the LTSA data to the sting vector
			}
			ltsa_export.close(); // Close the file
		}
	}
	return ltsa_exports;
}