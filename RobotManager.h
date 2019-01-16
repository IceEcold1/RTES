#ifndef ROBOT_MANAGER_H
#define ROBOT_MANAGER_H

#include <stdio.h>
#include "SynchronisationServer.h"
#include "HDS.h"
//#include "ArmManager.h"
//#include "LegManager.h"
//#include "SensorManager.h"
#include "FspProcess.h"
#include "posix-regex-cpp/POSIXRegex.h"
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

using namespace std;

struct ltsa_export {
	string process_id;
	vector<string> fsp_data;
};

class RobotManager {
public:
	//ArmManager *arm_manager;
	//LegManager *leg_manager;
	//SensorManager *sensor_manager;

	bool init_system();
	bool start_system();
private:
	vector<FspProcess*> processes;
	HDS *hds;
	SynchronisationServer *sync_server;

	vector<string> get_alphabet(vector<string> data);
	vector<ltsa_export> read_ltsa_exports();
};

#endif
