#ifndef HDS_H
#define HDS_H

#include <string>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include "Debug.h"
#include "FspProcess.h"
#include "CM730_Serial.h"

using namespace std;

/*Fill based on LTSA labels*/
enum darwin_string_command{
	action_not_found,
	no_action_needed,
	servo_rotate,
	sensor_read_x,
	sensor_read_y,
	sensor_read_z
};

/*HDS extends FspProcess*/
class HDS : public FspProcess{
private:
	CM730Serial *cm730_serial;
	int message_to_int(char* message);
	darwin_string_command str_to_enum(string const& action);
	string parse_action_value(string action);
	string parse_servo_sensor_id(string action);
	int next_action(string action);
	int result;
	string hds_action;
	bool transition_running;

public:
	/*Constructor, also gets the constructor from the super class*/
	HDS(string process_id, int state, vector<string> alphabet, vector<string> fspData);
	int execute_action(string action);
	void run();
};

#endif