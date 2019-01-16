#ifndef PROCESS_H
#define PROCESS_H

#include <atomic>
#include <string>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <stdlib.h>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "posix-regex-cpp/POSIXRegex.h"

using namespace std;

struct sens_list {
	int next_state;
	string action;
};

class SynchronisationServer;

class FspProcess {
private:
	string fsp_action;
protected:
	string process_id;
	int state;
	vector<string> alphabet, fspData;
	vector<sens_list> sensitivity_list;
	SynchronisationServer *sync_server;
	bool is_started;
	int get_next_state(string action);
public:
	int get_cur_state();
	bool next_action(string action);
	void run();
	FspProcess(string processID, int state, vector<string> alphabet, vector<string> fspData, SynchronisationServer *sync_server);
	vector<sens_list> compose_sensitivity_list(int state, vector<string> data);
	vector<string> get_alphabet();
	string get_process_id();
	bool alphabet_contains_action(string action);
	bool sensitivity_list_contains_action(string action);
	bool get_started_bool();
	vector<sens_list> get_sensitivity_list();
	void execute_action(string action);
};

#endif