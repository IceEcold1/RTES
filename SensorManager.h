#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "SynchronisationServer.h"

class SensorManager {
private:
	SynchronisationServer sync_server;
public:
	SensorManager(SynchronisationServer sync_server);
};

#endif