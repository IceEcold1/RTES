#ifndef HDS_H
#define HDS_H

#include <string.h>

struct command {
	int adress;
	int device_command;
	int value;
};

class HDS: public FspProcess {
private:
	void run();
	bool send_command_driver(command cmd);
	//command format_data(string state);
};

#endif