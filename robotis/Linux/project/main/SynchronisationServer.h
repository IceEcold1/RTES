#ifndef SYNCHRONISATION_SERVER_H
#define SYNCHRONISATION_SERVER_H

#include <string>
#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <unistd.h>

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

class SynchronisationServer {
private:
	//vector<struct process> processes;
	//vector<FspProcess> processes;


public:
	void run();
	void parse_process_data();
};
#endif