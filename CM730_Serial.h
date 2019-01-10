#ifndef CM730Serial_H
#define CM730Serial_H

#include <linux/serial.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h> 
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>

class CM730Serial
{
	public:
		CM730Serial(); 

		enum enum_methods {
			READ,
			WRITE_PAIR,
			WRITE
		};

		/* Initialise variables used for read and write*/
		int READ_LENGTH 		= 4;
		int READ_ACTION 		= 2;
		int READ_VALUE			= 2;
		
		int WRITE_LENGTH		= 5;
		int WRITE_ACTION		= 3;
		
		struct sub_cont_response{
			int length;
			char message[100];
		};

		sub_cont_response action(enum_methods method, int id, int address, int value = -1);
		
	private:
		/* Package indexes */
		enum packet_items {
			ID 			= 2,
			LENGTH		= 3,
			ACTION 		= 4,
			ADDRESS 	= 5,
			VALUE 		= 6,
			LOWBYTE 	= 6,
			HIGHBYTE 	= 7
		};

		int write_cm730(unsigned char* packet, int value, int length);
		int write_pair_cm730(unsigned char* packet, int value, int length);
		sub_cont_response read_cm730(unsigned char* packet, int length);

		unsigned char get_checksum(unsigned char* packet);
	
		/* File Descriptor waarnaar geschreven moet wroden */
		int USB;
};

#endif