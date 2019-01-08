#ifndef PROCESS_H
#define PROCESS_H


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

class FspProcess {

private:
	string process_id;
	int state;
	vector<string> alphabet, fspData;
	vector<sens_list> sensitivity_list;

	bool get_next_state(string action);
public:
	int get_cur_state();
	bool next_action(string action);
	void run();
	FspProcess(string processID, int state, vector<string> alphabet, vector<string> fspData);
	vector<sens_list> get_sensitivity_list(int state, vector<string> data);
};

#endif