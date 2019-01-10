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
#include "HDS.h"

using namespace std;

enum manager_id {
	arm_manager,
	leg_manager,
	sensor_manager,
	unknown
};

struct alphabet_process {
	string action;
	vector<FspProcess> processes;
};

struct manager_command {
	manager_id identifier;
	string action;
	bool resolved;
	int return_value;
};

class SynchronisationServer {
private:
	vector<alphabet_process> total_alphabet;
	vector<manager_command> action_list;
	HDS *hds;
	bool working;

	void collect_total_alphabet();
	bool action_exists_in_alphabet(string action);
	bool process_vector_contains_process(vector<FspProcess> process_vector, FspProcess process);
	bool action_is_valid(string action);

public:
	SynchronisationServer(HDS *hds);
	vector<FspProcess> processes;
	void run();
	void give_action(manager_command input);
	manager_command get_result(manager_command id);
};
#endif