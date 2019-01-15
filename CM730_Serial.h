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
		
		typedef enum {
			READ,
			READ_PAIR,
			WRITE,
			WRITE_PAIR
		} Actions;

		typedef struct {
			int 	length;
			int 	message;
		} Response;

		/* Initialise variables used for read and write*/
		static const int READ_LENGTH 		= 4;
		static const int READ_INSTRUCTION	= 2;
		static const int READ_VALUE			= 2;
		
		static const int WRITE_LENGTH		= 4;
		static const int WRITE_INSTRUCTION	= 3;

		static const int SINGLE_ARGUMENT_LENGTH = 4; // adres + arguments + 2;
		static const int DOUBLE_ARGUMENT_LENGTH = 5;

		static const int WRITE_PAIR_LENGTH	= 5;

		Response action(Actions method, int id, int address, int value = -1);
		void lock_torque();
		
	private:                          
		enum PacketItem {
			ID 			= 2,
			LENGTH		= 3,
			INSTRUCTION	= 4,
			ADDRESS 	= 5,
			VALUE 		= 6,
			DATA_LENGTH	= 6,
			LOWBYTE 	= 6,
			HIGHBYTE 	= 7
		};

		Response Read(unsigned char* packet, int packet_length);
		unsigned char getChecksum(unsigned char* packet);
	
		int USB;
		
		struct termios tty;
		struct serial_struct serinfo;
	
		
};

#endif