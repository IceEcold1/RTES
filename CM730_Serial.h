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
		
	private:
		enum class READ {
			Length 	= 4,
			Action 	= 2,
			Value 	= 2
		}
		
		enum class WRITE {
			Length 	= 5,
			Action	= 3
		}
		
		enum class WRITE_PAIR {
			Length 	= 5,
			Action 	= 3
		}
		
		typedef struct {
			READ 		read;
			WRITE 		write;
			WRITE_PAIR 	write_pair;
		} TYPE;
		
		enum PacketItem {
			ID 			= 2,
			LENGTH		= 3,
			ACTION 		= 4,
			ADDRESS 	= 5,
			VALUE 		= 6,
			LOWBYTE 	= 6,
			HIGHBYTE 	= 7
		}

		int action(TYPE type, int id, int address, int value = -1);
	
		int USB;
		
		struct termios tty;
		struct serial_struct serinfo;
		
		double baudrate = 1000000.0;
	
		
};

#endif