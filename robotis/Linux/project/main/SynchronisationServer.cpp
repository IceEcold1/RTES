#include "SynchronisationServer.h"

void SynchronisationServer::run()
{
	//std::string process[PROCESSES][STATE][ACTION_LABEL];
	//process
	this->read_ltsa_exports();
}

string*::SynchronisationServer::read_ltsa_exports()
{
	string* ltsa_exports;
	string data, path;
	DIR* dir = opendir("LTSA"); // Open the 'LTSA' directory containing the LTSA exports
	dirent* file;

	while((file = readdir(dir)) != NULL)
	{
		if(file->d_name[0] != '.')
		{
			ifstream ltsa_export;

			sprintf((char*)path.c_str(), "LTSA/%s", file->d_name); // String-format the path of the LTSA export file
			ltsa_export.open(path.c_str()); // Open the file using the generated path
			while(getline(ltsa_export, data)) // 
			{
				printf("%s\n", data.c_str());
			}
			if(ltsa_export.is_open())
				ltsa_export.close();
		}
	}
	return ltsa_exports;
}