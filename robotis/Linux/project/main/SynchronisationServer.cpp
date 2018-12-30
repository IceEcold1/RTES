#include "SynchronisationServer.h"

void SynchronisationServer::run()
{
	//std::string process[PROCESSES][STATE][ACTION_LABEL];
	//process
	//this->read_ltsa_exports();
}

vector<string> SynchronisationServer::read_ltsa_exports()
{
	vector<string> ltsa_exports;
	string data, tempdata, path;
	DIR* dir = opendir("LTSA"); // Open the 'LTSA' directory containing the LTSA exports
	dirent* file;

	while((file = readdir(dir)) != NULL)
	{
		if(file->d_name[0] != '.')
		{
			ifstream ltsa_export;

			sprintf((char*)path.c_str(), "LTSA/%s", file->d_name); // String-format the path of the LTSA export file
			ltsa_export.open(path.c_str()); // Open the file using the generated path
			while(getline(ltsa_export, tempdata)) // Read trough the selected file line by line
			{
				data += tempdata + "\n";
			}
			ltsa_exports.push_back(data);
			ltsa_export.close();
		}
	}
	return ltsa_exports;
}