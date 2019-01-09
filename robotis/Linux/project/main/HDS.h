#ifndef HDS_H
#define HDS_H

#include <string>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include "Debug.h"

using namespace std;


/*Fill based on LTSA labels*/
enum darwin_string_command{
	action_not_found,
	servo_rotate,
	sensor_read
};

struct command {
	int address;
	int device_command;
	int value;
};

/*HDS extends FspProcess*/
class HDS : public FspProcess{
private:
	bool send_command_driver(command cmd);
	command format_command(string state, int action_value);
	darwin_string_command str_to_enum(string const& action);
	int parse_action_value(string *action);
public:
	/*Constructor, also gets the constructor from the super class*/
	HDS(string process_id, int state, vector<string> alphabet, vector<string> fspData);
	bool next_action(string action);
	void run();
};

#endif