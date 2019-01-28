#ifndef SYNCHRONISATION_SERVER_H
#define SYNCHRONISATION_SERVER_H

#include <atomic>
#include <string>
#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <unistd.h>
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
	vector<FspProcess*> processes;
};

class FspProcess;

class SynchronisationServer {
private:
	vector<alphabet_process> total_alphabet;
	vector<FspProcess*> processes;
	HDS *hds;
	bool working;

	void collect_total_alphabet();
	bool action_exists_in_alphabet(string action);
	bool process_vector_contains_process(vector<FspProcess*> process_vector, FspProcess *process);
	bool action_is_valid(string action);
	void execute_action(string action);

public:
	SynchronisationServer(HDS *hds);
	void run();
	void add_process(FspProcess *process);
	void remove_process(string process_id);
};
#endif