#ifndef CM730Serial_H
#define CM730Serial_H

#include <linux/serial.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h> 
#include <stdio.h>
#include <sys/ioctl.h>

class CM730Serial
{
	public:
		CM730Serial(); 

		enum Methods {
			read1,
			write_pair1,
			write1
		};

		Method READ;
		Method WRITE;
		Method WRITE_PAIR;
		
	private:

		typedef struct {
			int id;
			int length;
			int action;
			int value;
		} Method;
		
		enum PacketItem {
			ID 			= 2,
			LENGTH		= 3,
			ACTION 		= 4,
			ADDRESS 	= 5,
			VALUE 		= 6,
			LOWBYTE 	= 6,
			HIGHBYTE 	= 7
		};

		int action(Method method, int id, int address, int value = -1);

		int Write(unsigned char* packet, int value);
		int WritePair(unsigned char* packet, int value);
		int Read(unsigned char* packet);

		unsigned char getChecksum(unsigned char* packet);
	
		int USB;
		
		struct termios tty;
		struct serial_struct serinfo;
		
		static constexpr double baudrate = 1000000.0;
	
		
};

#endif