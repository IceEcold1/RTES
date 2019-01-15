#include "HDS.h"


/*In C++ constructors of derived classes can only be called after the base class constructor is finished, and must contain the same variables.*/
HDS::HDS(string process_id, int state, vector<string> alphabet, vector<string> fspData) : FspProcess(process_id, state, alphabet, fspData)
{
	this->process_id = process_id;
	this->state = state;
	this->alphabet = alphabet;

	/*Standard values for sync server linking.*/
	this->hds_action = "NO_ACTION_SET";
	this->transition_running.store(false, memory_order_relaxed);
}

void::HDS::run()
{
	/*Init device driver class*/
	this->cm730_serial = new CM730Serial();
	/*Arm servo's so they can be used*/
	this->cm730_serial->action(this->cm730_serial->WRITE, 256, 24, 1);
	while(1)
	{
		usleep(200);
		/*Check if an action has to be made.*/
		if(strcmp(this->hds_action.c_str(), "NO_ACTION_SET") != 0)
		{
			printf("HDS::Action found (%s)\n", this->hds_action.c_str());
			this->result = this->next_action(this->hds_action);
			this->transition_running.store(false, memory_order_relaxed);
		}
	}
}

/*Do a next action, check if it's in the alphabet*/
/*Based on the result send a command to the driver, return a value*/
/*Return sensor value, -1 if errors are found or 0 if the servo has to be moved.*/
int::HDS::next_action(string action)
{
	int address = 0;
	darwin_string_command label = this->str_to_enum(action);
	string servo_sensor_id, action_value;

	this->parse_servo_sensor_id(action, servo_sensor_id);
	this->parse_action_value(action, action_value);

	switch(label){
		case action_not_found: 
				printf("HDS::Action is not added to enum/not found.\n");
			return -1;
		break;
		/*======================================================================*/
		case no_action_needed: 
				printf("HDS::No command is needed for this action.\n");
			return 0;
		break;
		/*======================================================================*/
		case servo_rotate:
			if(strcmp(action_value.c_str(), "NO_MATCHES_FOUND") == 0)
				return -1;
			printf("HDS::Servo (%s) rotates to position %s.\n", servo_sensor_id.c_str(), action_value.c_str());
			this->cm730_serial->action(this->cm730_serial->WRITE_PAIR, stoi(servo_sensor_id.erase(0, 1).c_str()), 30, stoi(action_value.c_str())).length;
			return 0;
		break;
		/*======================================================================*/
		case sensor_read_x:
			if(strcmp(action_value.c_str(), "NO_MATCHES_FOUND") == 0)
				return -1;
			printf("HDS::(%s) gets the %s (x) axis.\n", servo_sensor_id.c_str(), action_value.c_str());
			if(strcmp(servo_sensor_id.c_str(), "gyro") == 0)
			{
				address = 42;
			}
			else
			{
				address = 44;
			}
			return this->message_to_int(this->cm730_serial->action(this->cm730_serial->READ_PAIR, 200, address).message);
		break;
		/*======================================================================*/
		case sensor_read_y:
			if(strcmp(action_value.c_str(), "NO_MATCHES_FOUND") == 0)
					return -1;
			printf("HDS::(%s) gets the %s (y) axis.\n", servo_sensor_id.c_str(), action_value.c_str());
			if(strcmp(servo_sensor_id.c_str(), "gyro") == 0)
			{
				address = 40;
			}
			else
			{
				address = 46;
			}
			return this->message_to_int(this->cm730_serial->action(this->cm730_serial->READ_PAIR, 200, address).message);
		break;
		/*======================================================================*/
		case sensor_read_z:
			if(strcmp(action_value.c_str(), "NO_MATCHES_FOUND") == 0)
				return -1;
			printf("HDS::(%s) gets the %s (z) axis.\n", servo_sensor_id.c_str(), action_value.c_str());
			if(strcmp(servo_sensor_id.c_str(), "gyro") == 0)
			{
				address = 38;
			}
			else
			{
				address = 48;
			}
			return this->message_to_int(this->cm730_serial->action(this->cm730_serial->READ_PAIR, 200, address).message);
		break;
		/*======================================================================*/
		default:
			printf("HDS::Default case entered.\n");
			return -1;
		break;
	}
	return -1;
}

int::HDS::message_to_int(char* message)
{
	return atoi(message);
}

/*Generate enum based on string value for switch statement in C++*/
darwin_string_command HDS::str_to_enum(string action) {

    if (action.find("rotate_servo") != string::npos)
    	return servo_rotate;
    else if (action.find("get_data.x") != string::npos)
    	return sensor_read_x;
	else if (action.find("get_data.y") != string::npos)
		return sensor_read_y;
	else if (action.find("get_data.z") != string::npos)
		return sensor_read_z;
	else if (action.find("move_") != string::npos)
		return no_action_needed;

    return action_not_found;
}

/*Get the value from the action.*/
void HDS::parse_action_value(string action, string &action_value)
{
	POSIX::Regex regex;
	POSIX::Match match;
	regex.compile(".*?\\..*?\\.(.*)"); 
	match = regex.match(action);
	if(match.numGroups() > 0)
		action_value = match.group(1);
	else
		action_value = "NO_MATCHES_FOUND";
}

/*Get the servo id or sensor type from the action.*/
void HDS::parse_servo_sensor_id(string action, string &servo_sensor_id)
{
	POSIX::Regex regex;
	POSIX::Match match;
	regex.compile("(.*)\\..*\\..*"); 
	match = regex.match(action);
	if(match.numGroups() > 0)
		servo_sensor_id = match.group(1);
	else
		servo_sensor_id = "NO_MATCHES_FOUND";
}

/*Asynchronous action from sync server.*/
int HDS::execute_action(string action)
{
	this->hds_action = action;
	this->transition_running.store(true, memory_order_relaxed);

	while(this->transition_running.load(memory_order_relaxed))
	{
		usleep(200);/*Give time to load data*/
	}
	this->hds_action = "NO_ACTION_SET";
	printf("HDS::Action result: %d\n", this->result);
	return this->result;
}