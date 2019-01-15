#include "CommandFunction.h"

CommandFunction::CommandFunction(SynchronisationServer *sync_server, manager_id manager)
{
	this->sync_server = sync_server;
	this->manager = manager;
}

int CommandFunction::resolve_command(string action)
{
	manager_command cmd;

	cmd.identifier = this->manager;
	cmd.action = action;
	cmd.resolved = false;
	
	this->sync_server->give_action(cmd);

	while(!cmd.resolved)
		cmd = this->sync_server->get_result(cmd);
	if(cmd.successful)
		printf("CommandFunction::Action (%s), successfull\n", action.c_str());
	else
		printf("CommandFunction::Action (%s), NOT successfull\n", action.c_str());

	return cmd.return_value;
}