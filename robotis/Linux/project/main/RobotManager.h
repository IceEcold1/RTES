#ifndef ROBOT_MANAGER_H
#define ROBOT_MANAGER_H

#include <stdio.h>
#include "SynchronisationServer.h"
#include "HDS.h"
#include "ArmManager.h"
#include "LegManager.h"
#include "SensorManager.h"
#include "FspProcess.h"
#include "posix-regex-cpp/POSIXRegex.h"

using namespace std;

class RobotManager {
public:
	bool init_system();
private:
	vector<string> alphabets;

	bool start_system();
	vector<FspProcess> processes;
	vector<string> get_alphabet(vector<string> data);
	vector<string> get_sensitivity_list(int state, string data);
};

#endif
