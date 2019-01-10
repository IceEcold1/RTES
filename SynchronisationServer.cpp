#include "SynchronisationServer.h"

SynchronisationServer::SynchronisationServer()
{
	//init stuff
}

/*
 * Loops trough 
*/
void SynchronisationServer::run()
{
	this->collect_total_alphabet();
	this->main_loop();
}

void SynchronisationServer::main_loop()
{
	manager_command current_action;
	while(true)
	{
		if((int)this->action_list.size() > 0)
		{

			current_action = *this->action_list.begin();

			if(this->action_exists_in_alphabet(current_action.command))
			{
				current_action.return_value = this->notify_all_FSP_procs(current_action.command);
				current_action.resolved = true;

				/*put the result back in the list.data */
				this->completed_action_list.push_back(current_action);
				this->action_list.erase(this->action_list.begin());
			}
			else
			{
				current_action.resolved = false;
				this->completed_action_list.push_back(current_action);
				this->action_list.erase(this->action_list.begin());
			}
		}
	}
}

int SynchronisationServer::notify_all_FSP_procs(string action)
{
	//give the value to all all the procs and retrieve the return from the HDS 

	return 1;
}

void SynchronisationServer::collect_total_alphabet()
{
	int processes_size = (int)this->processes.size();

	for(int i = 0; i < processes_size; i++)
	{
		vector<string> alphabet = this->processes[i].get_alphabet();
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
			if(this->processes[j].alphabet_contains_action(this->total_alphabet[i].action) && !this->process_vector_contains_process(this->total_alphabet[i].processes, this->processes[j]))
				this->total_alphabet[i].processes.push_back(this->processes[j]);
		}
	}

	for(int i = 0; i < (int)this->total_alphabet.size(); i++)
	{
		for(int j = 0; j < (int)this->total_alphabet[i].processes.size(); j++)
		{
			printf("action[%d]: '%s', process[%d]: '%s'\n", i, this->total_alphabet[i].action.c_str(), j, this->total_alphabet[i].processes[j].get_process_id().c_str());
		}
	}
}

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

bool SynchronisationServer::process_vector_contains_process(vector<FspProcess> process_vector, FspProcess process)
{
	int size = (int)process_vector.size();

	for(int i = 0; i < size; i++)
	{
		if(strcmp(process_vector[i].get_process_id().c_str(), process.get_process_id().c_str()) == 0)
			return true;
	}
	return false;
}

bool SynchronisationServer::give_acion(manager_command input)
{
	try {
   		this->action_list.push_back(input);
   		return true;
	} catch (const std::bad_alloc& e){return false;}
}

manager_command SynchronisationServer::get_result(manager_command id)
{
	int i;
	manager_command *it;
	manager_command return_value;
	for(i = 0, it = this->completed_action_list.begin(); it != this->completed_action_list.end(); ++it, ++i)
	{
		if (it->identifier == id)
		{
			return_value = *(this->completed_action_list.begin()+i);
			this->completed_action_list.erase(this->completed_action_list.begin()+i);
			return return_value;
		}
	}
	
	return_value.identifier = (manager_id)3;
	return return_value;
}