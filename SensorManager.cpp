#include "SensorManager.h"

SensorManager::SensorManager(SynchronisationServer *sync_server)
{
	this->sync_server = sync_server;
}

sensor_data SensorManager::get_gyro_data()
{
	struct sensor_data gyro_data;
	return gyro_data;
}

sensor_data SensorManager::get_accelero_data()
{
	struct sensor_data accelero_data;
	return accelero_data;
}