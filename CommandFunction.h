#ifndef COMMAND_FUNCTION_H
#define COMMAND_FUNCTION_H

#include "SynchronisationServer.h"

using namespace std;

class CommandFunction{
private:
	manager_id manager;
	SynchronisationServer *sync_server;
public:
	int resolve_command(string action);
	CommandFunction(SynchronisationServer *sync_server, manager_id manager);
};


#endif
