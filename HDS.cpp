#include "HDS.h"

/*In C++ constructors of derived classes can only be called after the base class constructor is finished, and must contain the same variables.*/
HDS::HDS(string process_id, int state, vector<string> alphabet, vector<string> fspData) : FspProcess(process_id, state, alphabet, fspData)
{
	this->process_id = process_id;
	this->state = state;
	this->alphabet = alphabet;

	/*Standard values for sync server linking.*/
	this->hds_action = "";
	this->transition_running = false;
}

void::HDS::run()
{
	int action_return_val = 0;
	while(1)
	{
		usleep(1000000);
		if(DEBUG)
			printf("HDS, state: %d\n", this->state);
		/*Check if an action has to be made.*/
		if(strcmp(this->hds_action.c_str(), "") == 1)
		{
			action_return_val = this->next_action(this->hds_action);
			this->result = action_return_val;
			this->transition_running = false;
		}
	}
}

/*Do a next action, check if it's in the alphabet*/
/*Based on the result send a command to the driver, return a value*/
/*Return sensor value, -1 if errors are found or 0 if the servo has to be moved.*/
int::HDS::next_action(string action)
{
	/*Check if the element exists in the alphabet*/
	if(find(this->alphabet.begin(), this->alphabet.end(), action) != this->alphabet.end())
	{
		if(DEBUG)
			printf("HDS: Action %s found in alphabet", action.c_str());

		darwin_string_command label = this->str_to_enum(action);
		string servo_sensor_id = this->parse_servo_sensor_id(action);
		string action_value = this->parse_action_value(action);
		struct command driver_command;

		switch(label){
			case action_not_found: 
				if(DEBUG)
				{
					printf("Action is not added to enum/not found.\n");
				}
				return -1;
				break;
			case no_action_needed: 
				if(DEBUG)
				{
					printf("No command is needed for this action.\n");
				}
				return -1;
				break;
			case servo_rotate: 
				if(DEBUG)
				{
					printf("Servo %s rotates to position %s.\n", servo_sensor_id.c_str(), action_value.c_str());
				}
				driver_command = this->format_command(stoi(servo_sensor_id.erase(0, 1)), 30, stoi(action_value));
				break;
			case sensor_read_x:
				if(DEBUG)
				{
					printf("Sensor %s gets the %s (x) axis.\n", servo_sensor_id.c_str(), action_value.c_str());
				}
				//driver_command = this->format_command((int)servo_sensor_id.erase(0, 1), 30, (int)action_value);
				break;
			case sensor_read_y:
				if(DEBUG)
				{
					printf("Sensor %s gets the %s (y) axis.\n", servo_sensor_id.c_str(), action_value.c_str());
				}
				//driver_command = this->format_command((int)servo_sensor_id.erase(0, 1), 30, (int)action_value);
				break;
			case sensor_read_z:
				if(DEBUG)
				{
					printf("Sensor %s gets the %s (z) axis.\n", servo_sensor_id.c_str(), action_value.c_str());
				}
				//driver_command = this->format_command((int)servo_sensor_id.erase(0, 1), 30, (int)action_value);
				break;
			default:
					return -1;
				break;
		}
		return this->send_command_driver(driver_command);
	}
	else
	{
		return -1;
	}
}

/*Send the formatted command to the sub-controller*/
int::HDS::send_command_driver(command cmd)
{
	return 0;
}

/*Format command based on action label and value*/
command HDS::format_command(int id, int address, int action_value)
{
	struct command driver_command;
	driver_command.value = 0;
	return driver_command;
}

/*Generate enum based on string value for switch statement in C++*/
darwin_string_command HDS::str_to_enum(string const& action) {

    if (action.find("rotate") > 0)
    	return servo_rotate;
    else if (action.find("get_data.x") > 0)
    	return sensor_read_x;
	else if (action.find("get_data.y") > 0)
		return sensor_read_y;
	else if (action.find("get_data.z") > 0)
		return sensor_read_z;
	else if (action.find("move_") > 0)
		return no_action_needed;

    return action_not_found;
}

/*Get the value from the action.*/
string HDS::parse_action_value(string action)
{
	POSIX::Regex re;
	POSIX::Match m;
	re.compile("\\((\\w+)\\.\\w+\\.\\w+\\)"); 
	m = re.match(action);
	if(DEBUG)
		printf("Parse_action_value: %s\n", m.group(1).c_str());
	return m.group(1);
}

/*Get the servo id or sensor type from the action.*/
string HDS::parse_servo_sensor_id(string action)
{
	POSIX::Regex re;
	POSIX::Match m;
	re.compile("\\(\\w+\\.\\w+\\.(\\w+\\))"); 
	m = re.match(action);
	if(DEBUG)
		printf("Parse_servo_sensor_id: %s\n", m.group(1).c_str());
	return m.group(1);
}

/*Asynchronous action from sync server.*/
int HDS::execute_action(string action)
{
	this->hds_action = action;
	this->transition_running = true;

	while(this->transition_running == true)
	{
		/*do nothing*/
	}
	this->transition_running = false;

	return this->result;
}

