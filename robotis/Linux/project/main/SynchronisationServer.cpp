#include "SynchronisationServer.h"

void SynchronisationServer::run()
{
	//std::string process[PROCESSES][STATE][ACTION_LABEL];
	//process
	//this->read_ltsa_exports();
}

vector<struct ltsa_export> SynchronisationServer::read_ltsa_exports()
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
			while(getline(ltsa_export, data)) // Read trough the selected file line by line
			{
				export_data.fspData.push_back(data);
			}
			export_data.processID = file->d_name;
			ltsa_exports.push_back(export_data); // Add the LTSA data to the sting vector
			ltsa_export.close(); // Close the file
		}
	}
	return ltsa_exports;
}

void SynchronisationServer::parse_process_data()
{

}