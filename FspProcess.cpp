#include "FspProcess.h"
#include "SynchronisationServer.h"

/*Always include the initial state and full alphabet.*/
FspProcess::FspProcess(string process_id, int state, vector<string> alphabet, vector<string> fspData, SynchronisationServer *sync_server)
{
	this->process_id = process_id;
	this->state = state;
	this->alphabet = alphabet;
	this->fspData = fspData;
	this->sync_server = sync_server;
	this->sensitivity_list = this->compose_sensitivity_list(state, this->fspData);

	/*standard values*/
	this->fsp_action = "NO_ACTION_SET";
	this->is_started = false;
	this->is_busy.store(false,  memory_order_relaxed);
}

/*Check if the sync server sends a signal, based on that single do something.*/
void::FspProcess::run()
{
	this->is_started = true;
	while(1)
	{
		usleep(200);
		if(strcmp(this->fsp_action.c_str(), "NO_ACTION_SET") != 0 && this->is_busy.load(memory_order_relaxed))
		{
			this->next_action(this->fsp_action);
			this->fsp_action = "NO_ACTION_SET";
			this->is_busy.store(false,  memory_order_relaxed);
		}
	}
}

bool::FspProcess::get_busy()
{
	return this->is_busy.load(memory_order_relaxed);
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
		//printf("FspProcess::(%s), action (%s) found in alphabet.\n", this->process_id.c_str(), action.c_str());
		/*Also check if this action is possible based on the current state, actions based on states are found in the sensitivity list*/
		int next_state = this->get_next_state(action);
		if(next_state != -1)
		{
			//printf("FspProcess::(%s), action (%s) found in sensitivity list.\n", this->process_id.c_str(), action.c_str());
			printf("'%s': action: %s, old state: %d, new state: %d\n", action.c_str(), this->process_id.c_str(), this->state, next_state);
			this->state = next_state; /*New state*/
			this->sensitivity_list = this->compose_sensitivity_list(this->state, this->fspData);
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

vector<sens_list> FspProcess::compose_sensitivity_list(int state, vector<string> data)
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
int::FspProcess::get_next_state(string action)
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

bool FspProcess::get_started_bool()
{
	return this->is_started;
}

void FspProcess::execute_action(string action)
{
	this->is_busy.store(true,  memory_order_relaxed);
	this->fsp_action = action;
}

vector<sens_list> FspProcess::get_sensitivity_list()
{
	return this->sensitivity_list;
}