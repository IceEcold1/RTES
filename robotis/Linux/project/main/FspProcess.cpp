#include "FspProcess.h"

/*Always include the initial state and full alphabet.*/
FspProcess::FspProcess(string process_id, int state, vector<string> alphabet, vector<string> sensitivity_list)
{
	this->process_id = process_id;
	this->state = state;
	this->alphabet = alphabet;
	this->sensitivity_list = sensitivity_list;
}

/*Check if the sync server sends a signal, based on that single do something.*/
void::FspProcess::run()
{
	while(1)
	{
		usleep(1000000);
		printf("Process: '%s', state = %d\n", this->process_id.c_str(), this->state);
	}
}

/*Gets the current state which the actions will be based on.*/
int::FspProcess::get_cur_state()
{
	return this->state;
}

/*Do a next action, check if it's in the alphabet, update state*/
/*Based on the result return something to the sync server*/
bool::FspProcess::next_action(string action)
{
	/*Check if the element exists in the alphabet*/
	if(find(this->alphabet.begin(), this->alphabet.end(), action) != this->alphabet.end())
	{
		printf("Action %s found in alphabet", action.c_str());
		/*Also check if this action is possible based on the current state, actions based on states are found in the sensitivity list*/
		if(find(this->sensitivity_list.begin(), this->sensitivity_list.end(), action) != this->sensitivity_list.end())
		{
			printf("Action %s found in sensitivity list", action.c_str());
			this->state = /*New state*/;
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
