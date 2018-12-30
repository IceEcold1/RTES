#ifndef PROCESS_H
#define PROCESS_H


#include <string>
#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

class FspProcess{
private:
	int state;
	vector<string> alphabet;
public:
	int get_cur_state();
	bool next_action(string action);
	void run();
	FspProcess(int state, vector<string> alphabet);
};

#endif