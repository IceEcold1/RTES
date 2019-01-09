#include "FspProcess.h"
#include "HDS.h"

/*In C++ constructors of derived classes can only be called after the base class constructor is finished, and must contain the same variables*/
HDS::HDS(string process_id, int state, vector<string> alphabet, vector<string> fspData) : FspProcess(process_id, state, alphabet, fspData)
{
	this->process_id = process_id;
	this->state = state;
	this->alphabet = alphabet;
}

void::HDS::run()
{
	while(1)
	{
		usleep(1000000);
		printf("HDS, state: %d\n", this->state);
	}
}

/*Send the formatted command to the sub-controller*/
bool::HDS::send_command_driver(command cmd)
{
	return true;
}

/*Format command based on action label and value*/
command HDS::format_command(string action, int action_value)
{
	struct command driver_command;
	driver_command.value = 0;
	return driver_command;
}

/*Do a next action, check if it's in the alphabet, update state*/
/*Based on the result send a command to the driver, return the value*/

/*!!! return type, what does darwin return?!!!*/
bool::HDS::next_action(string action)
{
	/*Check if the element exists in the alphabet*/
	if(find(this->alphabet.begin(), this->alphabet.end(), action) != this->alphabet.end())
	{
		if(DEBUG)
			printf("HDS: Action %s found in alphabet", action.c_str());

		darwin_string_command label = this->str_to_enum(action);
		int action_value = this->parse_action_value(&action);
		switch(label){
			case action_not_found: printf("Action is not added to enum/not found.");
				break;
			case servo_rotate: printf("servo.rotate"); /*print for now, format command*/
				break;
			case sensor_read: printf("sensor.read"); /*print for now, format command*/
				break;
			default:
				/*Do Nothing*/
				break;
		}

		usleep(1000000); /*Wait for Darwin to respond, return value etc*/
		return true;
	}
	else
	{
		return false;
	}
}

/*Generate enum based on string value for switch statement in C++*/
darwin_string_command HDS::str_to_enum(string const& action) {
    if (action == "servo.rotate") return servo_rotate;
    if (action == "sensor.read") return sensor_read;

    return action_not_found;
}

/*Get the value from the action, remove the value from the action afterwards*/
int::HDS::parse_action_value(string *action)
{
	return 0;
}