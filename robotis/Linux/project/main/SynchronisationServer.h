#ifndef SYNCHRONISATION_SERVER_H
#define SYNCHRONISATION_SERVER_H

#include <string>
#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <unistd.h>
#include "FspProcess.h"

using namespace std;

struct alphabet_process {
	string action;
	vector<FspProcess> processes;
};

class SynchronisationServer {
private:
vector<alphabet_process> total_alphabet;
void collect_total_alphabet();
bool action_exists_in_alphabet(string action);

public:
	vector<FspProcess> processes;

	void run();
};
#endif