#ifndef HDS_H
#define HDS_H

#include <string>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include "Debug.h"

using namespace std;

struct command {
	int address;
	int device_command;
	int value;
};

/*HDS extends FspProcess*/
class HDS : public FspProcess{
private:
	bool send_command_driver(command cmd);
	command format_command(string state);
public:
	/*Constructor, also gets the constructor from the super class*/
	HDS(string process_id, int state, vector<string> alphabet, vector<string> fspData);
	bool next_action(string action);
	void run();
};

#endif