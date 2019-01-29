#include "HDS.h"


/*In C++ constructors of derived classes can only be called after the base class constructor is finished, and must contain the same variables.*/
HDS::HDS(string process_id, int state, vector<string> alphabet, vector<string> fspData) : FspProcess(process_id, state, alphabet, fspData, sync_server)
{
	this->process_id = process_id;
	this->state = state;
	this->alphabet = alphabet;

	/*Standard values for sync server linking.*/
	this->hds_action = "NO_ACTION_SET";
	this->sensitivity_list = this->compose_sensitivity_list(state, this->fspData);
	this->backup_sensitivity_list = this->sensitivity_list;
	this->is_busy.store(false, memory_order_relaxed);
}

void::HDS::run()
{
	/*Init device driver class*/
	this->cm730_serial = new CM730Serial();
	/*Arm servo's so they can be used*/
	this->cm730_serial->lock_torque();
	while(1)
	{
		usleep(200);
		/*Check if an action has to be made.*/
		if(strcmp(this->hds_action.c_str(), "NO_ACTION_SET") != 0 && this->is_busy.load(memory_order_relaxed))
		{
			this->result = this->next_action(this->hds_action);
			this->is_busy.store(false, memory_order_relaxed);
		}
	}
}

/*Do a next action, check if it's in the alphabet*/
/*Based on the result send a command to the driver, return a value*/
/*Return sensor value, -1 if errors are found or 0 if the servo has to be moved.*/
int HDS::next_action(string action)
{
	int address = 0;
	int sensor_value = 0;
	darwin_string_command label = this->str_to_enum(action);
	string servo_sensor_id, action_value;
	string return_action = "";

	int i = 0, dest_value = 0, response = 0, servo_id = 0;


	this->parse_servo_sensor_id(action, servo_sensor_id);
	this->parse_action_value(action, action_value);

	switch(label){
		case action_not_found: 
				printf("HDS::Action is not added to enum/not found.\n");
			return -1;
		break;
		/*======================================================================*/
		case servo_rotate:
			if(strcmp(action_value.c_str(), "NO_MATCHES_FOUND") == 0)
				return -1;

			printf("HDS::Servo (%s) rotates to position %s.\n", servo_sensor_id.c_str(), action_value.c_str());
			dest_value = stoi(action_value.c_str());
			servo_id = stoi(servo_sensor_id.erase(0, 1).c_str());
			this->cm730_serial->action(this->cm730_serial->WRITE_PAIR, servo_id, 30, dest_value);
	
			for(i = 0; i < 1000; i++) {
				response = this->cm730_serial->action(this->cm730_serial->READ_PAIR, servo_id, 36).message;

				if(response > (dest_value - 50) && response < (dest_value + 50)) break;

				usleep(1000);
			};

			if(i == 1000) printf("POSITION NOT REACHED");
			return 0;
		break;
		/*======================================================================*/
		case sensor_read_x:
			if(strcmp(action_value.c_str(), "NO_MATCHES_FOUND") == 0)
				return -1;

			if(strcmp(servo_sensor_id.c_str(), "accelero") == 0)
			{
				address = 42;
			}
			else
			{
				address = 44;
			}

			sensor_value = this->cm730_serial->action(this->cm730_serial->READ_PAIR, 200, address).message;
			printf("HDS::Sensor (%s) (x-axis) return value is: (%d).\n", servo_sensor_id.c_str(), sensor_value);

			this->set_sensitivity_list_returnval(sensor_value, servo_sensor_id, action_value);

			return sensor_value;
		break;
		/*======================================================================*/
		case sensor_read_y:
			if(strcmp(action_value.c_str(), "NO_MATCHES_FOUND") == 0)
					return -1;

			if(strcmp(servo_sensor_id.c_str(), "accelero") == 0)
			{
				address = 40;
			}
			else
			{
				address = 46;
			}

			sensor_value = this->cm730_serial->action(this->cm730_serial->READ_PAIR, 200, address).message;
			printf("HDS::Sensor (%s) (y-axis) return value is: (%d).\n", servo_sensor_id.c_str(), sensor_value);

			this->set_sensitivity_list_returnval(sensor_value, servo_sensor_id, action_value);

			return sensor_value;
		break;
		/*======================================================================*/
		case sensor_read_z:
			if(strcmp(action_value.c_str(), "NO_MATCHES_FOUND") == 0)
				return -1;

			if(strcmp(servo_sensor_id.c_str(), "accelero") == 0)
			{
				address = 38;
			}
			else
			{
				address = 48;
			}

			sensor_value = this->cm730_serial->action(this->cm730_serial->READ_PAIR, 200, address).message;
			printf("HDS::Sensor (%s) (z-axis) return value is: (%d).\n", servo_sensor_id.c_str(), sensor_value);

			this->set_sensitivity_list_returnval(sensor_value, servo_sensor_id, action_value);

			return sensor_value;

		break;
		/*======================================================================*/
		case return_data:
			if(strcmp(action_value.c_str(), "NO_MATCHES_FOUND") == 0)
				return -1;

			this->sensitivity_list = this->backup_sensitivity_list;

			return 0;

		break;
		/*======================================================================*/
		default:
			printf("HDS::Default case entered.\n");
			return -1;
		break;
	}
	return -1;
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
	else if(action.find("return_data") != string::npos)
		return return_data;

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
	this->is_busy.store(true, memory_order_relaxed);
	while(this->is_busy.load(memory_order_relaxed))
	{
		usleep(100);/*Give time to load data*/
	}
	this->hds_action = "NO_ACTION_SET";
	printf("HDS::Action result: %d\n", this->result);
	return this->result;
}

void HDS::set_sensitivity_list_returnval(int sensor_value, string sensor_id, string axis)
{
	string return_action = ".return_data.";
	return_action.insert(0, sensor_id);
	axis.append(".");
	return_action.append(axis);
	return_action = return_action + boost::lexical_cast<std::string>(sensor_value);

	vector<sens_list> temp_vect;
	sens_list temp_list;

	temp_list.next_state = 0;
	temp_list.action = return_action;
	temp_vect.push_back(temp_list);

	this->sensitivity_list = temp_vect;
	temp_vect.pop_back();
}