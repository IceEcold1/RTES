#ifndef HDS_H
#define HDS_H

#include <string>
#include <unistd.h>
#include <stdio.h>
#include <vector>

using namespace std;

struct command {
	int adress;
	int device_command;
	int value;
};

class HDS {
private:
	int state;
	vector<string> alphabet;
	vector<string> sensitivity_list;

	bool send_command_driver(command cmd);
	//command format_data(string state);
public:
	HDS(int state, vector<string> alphabets, vector<string> sensitivity_lists);
	void run();
};

#endif