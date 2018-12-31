#ifndef SYNCHRONISATION_SERVER_H
#define SYNCHRONISATION_SERVER_H

#include <string>
#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <fstream>
#include <vector>

using namespace std;

/*struct process {
	vector<struct node> nodes;
	char* processName;
}

struct node {
	int beginState;
	vector<transition> t;
}

struct transition {
	char *transName;
	int nextState;
}*/

struct ltsa_export {
	string processID;
	string fspData;
};

class SynchronisationServer {
private:
	//vector<struct process> processes;
	//vector<FspProcess> processes;


public:
	vector<ltsa_export> read_ltsa_exports();
	void run();
	void parse_process_data();
};
#endif