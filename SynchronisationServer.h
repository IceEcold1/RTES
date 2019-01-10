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

enum manager_id {
	Arm_manager,
	Leg_manager,
	Sensor_manager,
	unkown
};

struct alphabet_process {
	string action;
	vector<FspProcess> processes;
};

struct manager_command {
	manager_id identifier;
	string command;
	bool resolved;
	int return_value;
};

class SynchronisationServer {
private:
	vector<alphabet_process> total_alphabet;
	vector<manager_command> action_list;
	vector<manager_command> completed_action_list;
	bool working;

	void main_loop();
	int notify_all_FSP_procs(string action);
	void collect_total_alphabet();
	bool action_exists_in_alphabet(string action);
	bool process_vector_contains_process(vector<FspProcess> process_vector, FspProcess process);

public:
	SynchronisationServer();
	vector<FspProcess> processes;
	void run();
	bool give_acion(manager_command input);
	manager_command get_result(manager_command id);
};
#endif