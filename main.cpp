#include <stdio.h>
#include "RobotManager.h"
#include "CM730_Serial.h"

int main(void)
{
	CM730Serial cm730Serial;

	int red = 255 & 0xFF;
	int green = 0 & 0xFF;
	int blue = 0 & 0xFF;

	int color = (int)(((blue>>3)<<10)|((green>>3)<<5)|(red>>3));

	cm730Serial.action(cm730Serial.WRITE, 200, 26, color);

    //RobotManager manager;
    //manager.init_system();
    // Dit moet later omlaag maar kan nu niet omdat de main escaped vanwege ontbreken cm730 in VM
    //manager.start_system();
    //while(1) { }
    return 0;
}