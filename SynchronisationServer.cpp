#include "SynchronisationServer.h"

SynchronisationServer::SynchronisationServer(HDS *hds)
{
	this->hds = hds;
}

/*
 * checks if the action_list (FIFO) contains action(s)
 * if so, the function checks if the action exists in the 'total_alphabet' vector and if the action is valid
 * the ation is valid when the sensitivity_lists of the processes that are linked to the specified action contain the current action
 * then the hds executes the action
*/
void SynchronisationServer::run()
{
	atomic<int> size;
	int hds_result;
	this->collect_total_alphabet();
	while(1)
	{
		hds_result = 0;
		size.store((int)this->action_list.size(), memory_order_relaxed);
		if(size.load(memory_order_relaxed) > 0)
		{
			if(this->action_exists_in_alphabet(this->action_list[0].action) && action_is_valid(this->action_list[0].action) && !this->action_list[0].resolved)
			{
				hds_result = this->hds->execute_action(this->action_list[0].action);

				if(hds_result != -1)
				{
					int total_alphabet_size = (int)this->total_alphabet.size();
					for(int i = 0; i < total_alphabet_size; i++)
					{
						if(strcmp(this->action_list[0].action.c_str(), this->total_alphabet[i].action.c_str()) == 0)
						{
							int processes_size = (int)this->total_alphabet[i].processes.size();
							for(int j = 0; j < processes_size; j++)
							{
								this->total_alphabet[i].processes[j]->next_action(this->action_list[0].action);
							}
						}
					}
					this->action_list[0].return_value = hds_result;
					this->action_list[0].resolved = true;
					this->action_list[0].successful = true;
				}
				else
				{
					this->action_list[0].resolved = false;
					this->action_list[0].successful = false;
				}
			}
			else if(!this->action_list[0].resolved)
			{
				this->action_list[0].resolved = true;
				this->action_list[0].successful = false;
			}
		}
	}
}

/*
* Checks if the specified action exists in the sensitivity_lists of the processes that are linked to the specified action
*/
bool SynchronisationServer::action_is_valid(string action)
{
	int size = (int)this->total_alphabet.size();

	for(int i = 0; i < size; i++)
	{
		int process_size = (int)this->total_alphabet[i].processes.size();

		if(strcmp(this->total_alphabet[i].action.c_str(), action.c_str()) == 0)
		{
			for(int j = 0; j < process_size; j++)
			{
				if(!this->total_alphabet[i].processes[j]->sensitivity_list_contains_action(this->total_alphabet[i].action))
					return false;
			}
		}
	}
	return true;
}

/*
* Loops trough all the processes and makes a list with unique actions
* Every item in the list contains an action and the processes linked that contain the action
*/
void SynchronisationServer::collect_total_alphabet()
{
	int processes_size = (int)this->processes.size();

	for(int i = 0; i < processes_size; i++)
	{
		vector<string> alphabet = this->processes[i]->get_alphabet();
		int alphabet_size = (int)alphabet.size();

		for(int j = 0; j < alphabet_size; j++)
		{
			struct alphabet_process alphabet_item;

			if(!this->action_exists_in_alphabet(alphabet[j]))
			{
				alphabet_item.action = alphabet[j];
				alphabet_item.processes.push_back(this->processes[i]);
			}
			this->total_alphabet.push_back(alphabet_item);
		}
	}

	int total_alphabet_size = (int)this->total_alphabet.size();
	for(int i = 0; i < total_alphabet_size; i++)
	{
		for(int j = 0; j < processes_size; j++)
		{
			if(this->processes[j]->alphabet_contains_action(this->total_alphabet[i].action) && !this->process_vector_contains_process(this->total_alphabet[i].processes, this->processes[j]))
				this->total_alphabet[i].processes.push_back(this->processes[j]);
		}
	}

	/*for(int i = 0; i < (int)this->total_alphabet.size(); i++)
	{
		for(int j = 0; j < (int)this->total_alphabet[i].processes.size(); j++)
		{
			printf("action[%d]: '%s', process[%d]: '%s'\n", i, this->total_alphabet[i].action.c_str(), j, this->total_alphabet[i].processes[j].get_process_id().c_str());
		}
	}*/
}

/*
* Returns wether the specified action contains in the locally defubed 'total_alphabet' vector
*/
bool SynchronisationServer::action_exists_in_alphabet(string action)
{
	int size = (int)this->total_alphabet.size();
	for(int i = 0; i < size; i++)
	{
		if(strcmp(this->total_alphabet[i].action.c_str(), action.c_str()) == 0)
			return true;
	}
	return false;
}

/*
* Returns wether the specified process exists in the specified process_vector
*/
bool SynchronisationServer::process_vector_contains_process(vector<FspProcess*> process_vector, FspProcess *process)
{
	int size = (int)process_vector.size();

	for(int i = 0; i < size; i++)
	{
		if(strcmp(process_vector[i]->get_process_id().c_str(), process->get_process_id().c_str()) == 0)
			return true;
	}
	return false;
}

/*
* Public function to add a command to the FIFO-buffer from the managers
*/
void SynchronisationServer::give_action(manager_command input)
{
	this->action_list.push_back(input);
}

/*
* Returns the result of an action
* Checks if the identifier of the action in the FIFO is equal to the identifier in the specified manager_command (struct)
*/
manager_command SynchronisationServer::get_result(manager_command id)
{
	int size = (int)this->action_list.size();
	manager_command return_value;

	for(int i = 0; i < size; i++)
	{
		if (this->action_list[i].identifier == id.identifier)
		{
			return_value = this->action_list[i];
			if(return_value.resolved)
				this->action_list.erase(this->action_list.begin()+i);
			return return_value;
		}
	}
	
	return_value.identifier = (manager_id)unknown;
	return return_value;
}