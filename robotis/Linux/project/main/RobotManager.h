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
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

using namespace std;

class RobotManager {
public:
	bool init_system();
	bool start_system();
private:
	vector<FspProcess> processes;
	vector<string> get_alphabet(vector<string> data);
	vector<string> get_sensitivity_list(int state, vector<string> data);
};

#endif
