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
	int size = (int)this->processes.size();

	this->collect_total_alphabet();
	while(1)
	{
		for(int i = 0; i < size && !this->processes[i]->get_busy(); i++)
		{
			vector<sens_list> sensitivity_list = this->processes[i]->get_sensitivity_list();
			int sensitivity_list_size = (int)sensitivity_list.size();

			for(int j = 0; j < sensitivity_list_size; j++)
			{
				if(this->action_is_valid(sensitivity_list[j].action))
				{
					this->execute_action(sensitivity_list[j].action);
				}
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
	int process_size = 0;
	int i;

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
	/*Loop door processen*/
	for(int j = 0; j < process_size; j++)
	{
		/*Loop door alle processen gekoppeld aan een enkele actie en check of deze processen deze actie in hun sens_list hebben*/
		if(this->total_alphabet[i].processes[j]->get_busy() || !this->total_alphabet[i].processes[j]->sensitivity_list_contains_action(this->total_alphabet[i].action))
			return false;
	}
	if(this->hds->alphabet_contains_action(action) && (!this->hds->sensitivity_list_contains_action(action) && this->hds->get_busy()))
		return false;
	return true;
}

/*
* Loops trough all the processes and makes a list with unique actions
* Every item in the list contains an action and the processes linked that contain the action
*/
void SynchronisationServer::collect_total_alphabet()
{
	int processes_size = (int)this->processes.size(); // size of vector 'processes'

	for(int i = 0; i < processes_size; i++)	// loop trough each process
	{
		vector<string> alphabet = this->processes[i]->get_alphabet(); // get alphabet of the current process
		int alphabet_size = (int)alphabet.size(); // get size of the alphabet of the current process

		for(int j = 0; j < alphabet_size; j++) // loop trough all actions
		{
			struct alphabet_process alphabet_item; // define new struct for each action

			if(!this->action_exists_in_alphabet(alphabet[j])) // check if action exists in the current alphabet
			{
				alphabet_item.action = alphabet[j]; // set the action in the struct
				alphabet_item.processes.push_back(this->processes[i]); // push the current process to the process vector of the struct
				this->total_alphabet.push_back(alphabet_item); // push the struct to the vector (total_alphabet)
			}
		}
	}

	int total_alphabet_size = (int)this->total_alphabet.size(); // get the size of total_alphabet
	for(int i = 0; i < total_alphabet_size; i++) // loop trough each item of total_alphabet
	{
		for(int j = 0; j < processes_size; j++) // loop trhough each process
		{
			if(this->processes[j]->alphabet_contains_action(this->total_alphabet[i].action) && !this->process_vector_contains_process(this->total_alphabet[i].processes, this->processes[j]))
				this->total_alphabet[i].processes.push_back(this->processes[j]);
		}
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

/*void SynchronisationServer::remove_process(string process_id)
{
	int processes_size = (int)this->processes.size();

	for(int i = 0; i < processes_size; i++)
	{
		if(strcmp(this->processes[i]->get_process_id().c_str(), process_id.c_str()) == 0)
			this->processes.erase(this->processes.begin() + i);
	}
}*/

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
			{
				this->total_alphabet[i].processes[j]->execute_action(action);
			}
			/*check for hardware action*/
			if(this->hds->alphabet_contains_action(action))
			{
				this->hds->execute_action(action);
			}
		}	
	}
}