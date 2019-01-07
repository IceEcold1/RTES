#include "HDS.h"

HDS::HDS(int state, vector <string> alphabet, vector<string> sensitivity_list)
{
	this->state = state;
	this->alphabet = alphabet;
	this->sensitivity_list = sensitivity_list;
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

//command format_data(string state)
//{

//}