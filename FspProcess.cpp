#include "FspProcess.h"

/*Always include the initial state and full alphabet.*/
FspProcess::FspProcess(string process_id, int state, vector<string> alphabet, vector<string> fspData)
{
	this->process_id = process_id;
	this->state.store(state, memory_order_relaxed);
	this->alphabet = alphabet;
	this->fspData = fspData;
	this->sensitivity_list = this->get_sensitivity_list(state, this->fspData);

	/*standard values*/
	this->is_started = false;
}

/*Check if the sync server sends a signal, based on that single do something.*/
void::FspProcess::run()
{
	this->is_started = true;
	while(1)
	{
		usleep(1000000);
		printf("Process: '%s', state = %d\n", this->process_id.c_str(), this->state.load(memory_order_relaxed));
	}
}

/*Gets the current state which the actions will be based on.*/
int::FspProcess::get_cur_state()
{
	return this->state.load(memory_order_relaxed);
}

/*Do a next action, check if it's in the alphabet, update state*/
/*Based on the result return something to the sync server*/
bool::FspProcess::next_action(string action)
{
	/*Check if the element exists in the alphabet*/
	if(find(this->alphabet.begin(), this->alphabet.end(), action) != this->alphabet.end())
	{
		printf("FspProcess: '%s', action %s found in alphabet\n", this->process_id.c_str(), action.c_str());
		/*Also check if this action is possible based on the current state, actions based on states are found in the sensitivity list*/
		int next_state = this->get_next_state(action);
		if(next_state != -1)
		{
			printf("FspProcess: '%s', action %s found in sensitivity list\n", this->process_id.c_str(), action.c_str());
			this->state.store(next_state, memory_order_relaxed); /*New state*/
			this->sensitivity_list = this->get_sensitivity_list(this->state.load(memory_order_relaxed), this->fspData);
			// hier de sens_list setten in de sync server
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

vector<sens_list> FspProcess::get_sensitivity_list(int state, vector<string> data)
{
	POSIX::Regex re;
	POSIX::Match m;
	vector<struct sens_list> res;

	int size = (int)data.size();

	for(int i = 1; i < size; i++)
	{
		re.compile("\\((.*?),.*?,.*?\\)"); // Regex for the first parameter of the data
		m = re.match(data[i]);
		if(atoi(m.group(1).c_str()) == state)
		{
			struct sens_list new_sens_list;

			re.compile("\\(.*?,.*?,(.*?)\\)"); // Regex for the third parameter of the
			m = re.match(data[i]);
			new_sens_list.next_state = atoi(m.group(1).c_str());

			re.compile("\\(.*?,(.*?),.*?\\)"); // Regex for the second parameter of the data
			m = re.match(data[i]);
			new_sens_list.action = m.group(1);

			res.push_back(new_sens_list);
		}
	}
	return res;
}

/*
 * If return value is -1 -> action is not found in sensitivity list
 * Else the return value is the next state
*/
bool::FspProcess::get_next_state(string action)
{
	for(int i = 0; i < (int)this->sensitivity_list.size(); i++)
	{
		if(strcmp(this->sensitivity_list[i].action.c_str(), action.c_str()) == 0)
			return this->sensitivity_list[i].next_state;
	}
	return -1;
}

vector<string> FspProcess::get_alphabet()
{
	return this->alphabet;
}

string FspProcess::get_process_id()
{
	return this->process_id;
}

bool FspProcess::alphabet_contains_action(string action)
{
	int size = (int)this->alphabet.size();

	for(int i = 0; i < size; i++)
	{
		if(strcmp(action.c_str(), this->alphabet[i].c_str()) == 0)
			return true;
	}
	return false;
}

bool FspProcess::sensitivity_list_contains_action(string action)
{
	int size = (int)this->sensitivity_list.size();

	for(int i = 0; i < size; i++)
	{
		if(strcmp(action.c_str(), this->sensitivity_list[i].action.c_str()) == 0)
			return true;
	}
	return false;
}

bool FspProcess::get_started_bool(){
	return this->is_started;
}