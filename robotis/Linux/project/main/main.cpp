/*
 * main.cpp
 *
 *  Created on: 2011. 1. 4.
 *      Author: robotis
 */

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <libgen.h>
#include <signal.h>

#include <rtai.h>
#include <rtai_sched.h>

#include "mjpg_streamer.h"
#include "LinuxDARwIn.h"
#include "RobotManager.h"

#ifdef MX28_1024
#define MOTION_FILE_PATH    "../../../Data/motion_1024.bin"
#else
#define MOTION_FILE_PATH    "../../../Data/motion_4096.bin"
#endif

#define SCRIPT_FILE_PATH    "script.asc"

#define U2D_DEV_NAME0       "/dev/ttyUSB0"
#define U2D_DEV_NAME1       "/dev/ttyUSB1"
void move_arm(int val);
LinuxCM730 linux_cm730(U2D_DEV_NAME0);
CM730 cm730(&linux_cm730);

void change_current_dir()
{
    char exepath[1024] = {0};
    if(readlink("/proc/self/exe", exepath, sizeof(exepath)) != -1)
    {
        if(chdir(dirname(exepath)))
            fprintf(stderr, "chdir error!! \n");
    }
}

void sighandler(int sig)
{
    exit(0);
}

int main(void)
{
    RobotManager manager;
    manager.init_system();

    // Dit moet later omlaag maar kan nu niet omdat de main escaped vanwege ontbreken cm730 in VM
    manager.start_system();
    while(1){}

    signal(SIGABRT, &sighandler);
    signal(SIGTERM, &sighandler);
    signal(SIGQUIT, &sighandler);
    signal(SIGINT, &sighandler);

    change_current_dir();

    //////////////////// Framework Initialize ////////////////////////////
    if(MotionManager::GetInstance()->Initialize(&cm730) == false)
    {
        linux_cm730.SetPortName(U2D_DEV_NAME1);
        if(MotionManager::GetInstance()->Initialize(&cm730) == false)
        {
            printf("Fail to initialize Motion Manager!\n");
            return 0;
        }
    }

    MotionManager::GetInstance()->AddModule((MotionModule*)Action::GetInstance());
    MotionManager::GetInstance()->AddModule((MotionModule*)Head::GetInstance());
    MotionManager::GetInstance()->AddModule((MotionModule*)Walking::GetInstance());

    LinuxMotionTimer *motion_timer = new LinuxMotionTimer(MotionManager::GetInstance());
    motion_timer->Start();
    /////////////////////////////////////////////////////////////////////

    int firm_ver = 0;
    if(cm730.ReadByte(JointData::ID_HEAD_PAN, MX28::P_VERSION, &firm_ver, 0)  != CM730::SUCCESS)
    {
        fprintf(stderr, "Can't read firmware version from Dynamixel ID %d!! \n\n", JointData::ID_HEAD_PAN);
        exit(0);
    }


    Action::GetInstance()->LoadFile((char*)MOTION_FILE_PATH);

    Action::GetInstance()->m_Joint.SetEnableBody(true, true);
    MotionManager::GetInstance()->SetEnable(true);

    cm730.WriteByte(CM730::P_LED_PANNEL, 0x01|0x02|0x04, NULL);

    //LinuxActionScript::PlayMP3("../MaxIsGay.mp3");
    Action::GetInstance()->Start(15);
    while(Action::GetInstance()->IsRunning()) usleep(8*1000);
	
	printf("Hi\n");
	sleep(4);
	move_arm(JointData::ID_L_SHOULDER_PITCH);
    sleep(2);
    move_arm(JointData::ID_R_SHOULDER_PITCH);
    //manager.start_system();
    //while(1){}
    return 0;
}

void move_arm(int val)
{
	if(cm730.Connect() == false)
	{printf("Fail to connect CM-730!\n");return;}
	
	printf("Moving Arm\n");
	printf("%i\n", MX28::P_TORQUE_ENABLE);

    cm730.WriteWord(val, 24, 10, 0);
}

