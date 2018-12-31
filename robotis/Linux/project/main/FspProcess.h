#ifndef PROCESS_H
#define PROCESS_H


#include <string>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

class FspProcess{
private:
	string processID;
	int state;
	vector<string> alphabet;
	vector<string> sensitivityList;
public:
	int get_cur_state();
	bool next_action(string action);
	void run();
	FspProcess(string processID, int state, vector<string> alphabet, vector<string> sensitivityList);
};

#endif