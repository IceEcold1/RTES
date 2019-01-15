#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "SynchronisationServer.h"
#include "CommandFunction.h"

struct sensor_data {
	int x, y, z;
};

class SensorManager {
private:
	SynchronisationServer *sync_server;
	CommandFunction *command_function;
public:
	SensorManager(SynchronisationServer *sync_server);
	sensor_data get_gyro_data();
	sensor_data get_accelero_data();
};

#endif