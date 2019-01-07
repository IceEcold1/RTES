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

using namespace std;

class FspProcess {
private:
	string process_id;
	int state;
	vector<string> alphabet;
	vector<string> sensitivity_list;
public:
	int get_cur_state();
	bool next_action(string action);
	void run();
	//FspProcess(string processID, int state, vector<string> alphabet, vector<string> sensitivityList);
	FspProcess(string processID, int state, vector<string> alphabet, vector<string> sensitivity_list);
};

#endif