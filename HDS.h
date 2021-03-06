#ifndef HDS_H
#define HDS_H

#include <atomic>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include "FspProcess.h"
#include "CM730_Serial.h"

using namespace std;

/*Fill based on LTSA labels*/
enum darwin_string_command{
	action_not_found,
	servo_rotate,
	sensor_read_x,
	sensor_read_y,
	sensor_read_z,
	return_data
};

/*HDS extends FspProcess*/
class HDS : public FspProcess{
private:
	CM730Serial *cm730_serial;
	darwin_string_command str_to_enum(string action);
	void parse_action_value(string action, string &action_value);
	void parse_servo_sensor_id(string action, string &servo_sensor_id);
	string servo_sensor_id, action_value;
	int next_action(string action);
	int result;
	string hds_action;
	vector<sens_list> backup_sensitivity_list;
	void set_sensitivity_list_returnval(int sensor_value, string sensor_id, string axis);

public:
	/*Constructor, also gets the constructor from the super class*/
	HDS(string process_id, int state, vector<string> alphabet, vector<string> fspData);
	int execute_action(string action);
	void run();
};

#endif