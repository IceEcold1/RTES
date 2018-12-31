#ifndef ROBOT_MANAGER_H
#define ROBOT_MANAGER_H

#include <stdio.h>
//#include <thread>
#include <tr1/regex>
#include "SynchronisationServer.h"
#include "HDS.h"
#include "ArmManager.h"
#include "LegManager.h"
#include "SensorManager.h"
#include "FspProcess.h"

using namespace std::tr1;

typedef match_results<const char*> cmatch;

class RobotManager {
public:
	bool init_system();
private:
	bool start_system();
	vector<FspProcess> processes;
	vector<string> get_alphabet(string data);
	vector<string> get_sensitivity_list(int state, string data);
};

#endif
