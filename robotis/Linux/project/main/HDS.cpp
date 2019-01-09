#include "FspProcess.h"
#include "HDS.h"

HDS::HDS(string process_id, int state, vector<string> alphabet, vector<string> fspData) : FspProcess(process_id, state, alphabet, fspData)
{
	this->process_id = process_id;
	this->state = state;
	this->alphabet = alphabet;
	this->fspData = fspData;
	this->sensitivity_list = this->get_sensitivity_list(this->state, this->fspData);
}

void::HDS::run()
{
	while(1)
	{
		usleep(1000000);
		printf("HDS, state: %d\n", this->state);
	}
}

bool::HDS::send_command_driver(command cmd)
{
	return true;
}

command HDS::format_command(string action)
{
	struct command driver_command;
	driver_command.value = 0;
	return driver_command;
}

/*Do a next action, check if it's in the alphabet, update state*/
/*Based on the result return something to the sync server*/
bool::HDS::next_action(string action)
{
	/*Check if the element exists in the alphabet*/
	if(find(this->alphabet.begin(), this->alphabet.end(), action) != this->alphabet.end())
	{
		if(DEBUG)
			printf("HDS: Action %s found in alphabet", action.c_str());
		/*Also check if this action is possible based on the current state, actions based on states are found in the sensitivity list*/
		int next_state = this->get_next_state(action);
		if(next_state != -1)
		{
			if(DEBUG)
				printf("HDS: Action %s found in sensitivity list", action.c_str());
			this->state = next_state; /*New state*/
			this->sensitivity_list = this->get_sensitivity_list(this->state, this->fspData);
			// hier de sens_list setten in de sync server
			//stuur commando
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}