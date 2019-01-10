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
			READ,
			WRITE_PAIR,
			WRITE
		};

		/* Variabelen initaliseren mbt op READ en WRITE values. */
		int READ_LENGTH 		= 4;
		int READ_ACTION 		= 2;
		int READ_VALUE			= 2;
		
		int WRITE_LENGTH		= 5;
		int WRITE_ACTION		= 3;
		
		struct sub_cont_response{
			int length;
			char message[100];
		};

		sub_cont_response action(Methods method, int id, int address, int value = -1);
		
	private:
		/* pakket indexes initaliseren */
		enum PacketItem {
			ID 			= 2,
			LENGTH		= 3,
			ACTION 		= 4,
			ADDRESS 	= 5,
			VALUE 		= 6,
			LOWBYTE 	= 6,
			HIGHBYTE 	= 7
		};

		int Write(unsigned char* packet, int value, int length);
		int WritePair(unsigned char* packet, int value, int length);
		sub_cont_response Read(unsigned char* packet, int length);

		unsigned char getChecksum(unsigned char* packet);
	
		/* File Descriptor waarnaar geschreven moet wroden */
		int USB;
};

#endif