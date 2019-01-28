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
	bool exit;

	this->collect_total_alphabet();
	while(1)
	{
		exit = false;
		hds_result = 0;
		size.store((int)this->processes.size(), memory_order_relaxed);

		for(int i = 0; i < size.load(memory_order_relaxed); i++)
		{
			if(exit) break;
			vector<sens_list> sensitivity_list = this->processes[i]->get_sensitivity_list();
			int sensitivity_list_size = (int)sensitivity_list.size();

			for(int j = 0; j < sensitivity_list_size; j++)
			{
				if(this->action_is_valid(sensitivity_list[j].action))
				{
					this->execute_action(sensitivity_list[j].action);
					usleep(1000000);
					exit = true;
					break;
				}
				usleep(1000000);
			}
			size.store((int)this->processes.size(), memory_order_relaxed);
		}
		//usleep(500000);
	}
}

/*
* Checks if the specified action exists in the sensitivity_lists of the processes that are linked to the specified action
*/
bool SynchronisationServer::action_is_valid(string action)
{
	int size = (int)this->total_alphabet.size();
	int process_size = 0;
	int i;

	printf("%s\n", action.c_str());

	if(size == 0)
		return false;
	for(i = 0; i < size; i++)
	{
		if(strcmp(this->total_alphabet[i].action.c_str(), action.c_str()) == 0)
		{
			process_size = (int)this->total_alphabet[i].processes.size();
			break;
		}
		else if(i == size - 1)
			return false;
	}
	for(int j = 0; j < process_size; j++)
	{
		if(!this->total_alphabet[i].processes[j]->sensitivity_list_contains_action(this->total_alphabet[i].action))
			return false;
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
	for(int i = 0; i < total_alphabet_size; i++)
	{
		//printf("action %d/%d: %s\n", i, total_alphabet_size, this->total_alphabet[i].action.c_str());
	}
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

void SynchronisationServer::remove_process(string process_id)
{
	int processes_size = (int)this->processes.size();

	for(int i = 0; i < processes_size; i++)
	{
		if(strcmp(this->processes[i]->get_process_id().c_str(), process_id.c_str()) == 0)
			this->processes.erase(this->processes.begin() + i);
	}
}

void SynchronisationServer::add_process(FspProcess *process)
{
	this->processes.push_back(process);
	printf("SynchronisationServer::add_process(): %s, processes.size(): %d\n", process->get_process_id().c_str(), (int)this->processes.size());
}

void SynchronisationServer::execute_action(string action)
{
	int total_alphabet_size = (int)this->total_alphabet.size();

	for(int i = 0; i < total_alphabet_size; i++)
	{
		if(strcmp(this->total_alphabet[i].action.c_str(), action.c_str()) == 0)
		{
			int process_size = (int)this->total_alphabet[i].processes.size();
			for(int j = 0; j < process_size; j++)
				this->total_alphabet[i].processes[j]->execute_action(action);
		}
	}
}