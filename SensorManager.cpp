#include "SensorManager.h"

SensorManager::SensorManager(SynchronisationServer *sync_server)
{
	this->command_function = new CommandFunction(sync_server, (manager_id)arm_manager);
}

sensor_data SensorManager::get_gyro_data()
{
	struct sensor_data gyro_data;
	gyro_data.x = this->command_function->resolve_command("gyro.get_data.x");
	printf("\n");
	gyro_data.y = this->command_function->resolve_command("gyro.get_data.y");
	printf("\n");
	gyro_data.z = this->command_function->resolve_command("gyro.get_data.z");
	printf("<!----------------Gyro data: x:%d y:%d z:%d\n",gyro_data.x, gyro_data.y, gyro_data.z);
	printf("\n");
	return gyro_data;
}

sensor_data SensorManager::get_accelero_data()
{
	struct sensor_data accelero_data;
	accelero_data.x = this->command_function->resolve_command("accelero.get_data.x");
	printf("\n");
	accelero_data.y = this->command_function->resolve_command("accelero.get_data.y");
	printf("\n");
	accelero_data.z = this->command_function->resolve_command("accelero.get_data.z");
	printf("<!----------------Accelero data: x:%d y:%d z:%d\n",accelero_data.x, accelero_data.y, accelero_data.z);
	printf("\n");

	return accelero_data;
}