#include "CM730_Serial.h"

CM730Serial::CM730Serial() {
	double baudrate = 1000000.0;

	this->USB = open("/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NONBLOCK);
	
	cfsetospeed(&this->tty, (speed_t)baudrate);
	cfsetispeed(&this->tty, (speed_t)baudrate);
	
	this->tty.c_cflag      = B38400|CS8|CLOCAL|CREAD;
	this->tty.c_iflag      = IGNPAR;
	this->tty.c_oflag      = 0;
	this->tty.c_lflag      = 0;
	this->tty.c_cc[VTIME]  = 0;
	this->tty.c_cc[VMIN]   = 0;

	ioctl(USB, TIOCGSERIAL, &this->serinfo);

	this->serinfo.flags &= ~ASYNC_SPD_MASK;
    this->serinfo.flags |= ASYNC_SPD_CUST;
    this->serinfo.custom_divisor = this->serinfo.baud_base / baudrate;

   	ioctl(this->USB, TIOCSSERIAL, &this->serinfo);
	
	cfmakeraw(&this->tty);
	
	tcflush(this->USB, TCIFLUSH);
	
	tcsetattr(this->USB, TCSANOW, &this->tty);
}


void CM730Serial::lock_torque() {
	this->action(WRITE, 254, 24, 1);
	usleep(100000);
	this->action(WRITE, 254, 24, 1);
}

CM730Serial::Response CM730Serial::action(Actions action, int id, int address, int value) {
	unsigned char instruction_packet[9];

	instruction_packet[0] = 0xFF;
	instruction_packet[1] = 0xFF;

	instruction_packet[ID]			= (unsigned char)id;
	instruction_packet[ADDRESS]		= (unsigned char)address;

	tcflush(this->USB, TCIFLUSH);

	Response response;

	switch(action) {
		case READ:
			instruction_packet[LENGTH]			= (unsigned char)SINGLE_ARGUMENT_LENGTH;
			instruction_packet[INSTRUCTION] 	= (unsigned char)READ_INSTRUCTION;
			instruction_packet[DATA_LENGTH]		= 1;
			instruction_packet[DATA_LENGTH + 1] = this->getChecksum(instruction_packet);

			response = this->Read(instruction_packet, 8);
			break;
		case READ_PAIR:
			instruction_packet[LENGTH]			= (unsigned char)SINGLE_ARGUMENT_LENGTH;
			instruction_packet[INSTRUCTION] 	= (unsigned char)READ_INSTRUCTION;
			instruction_packet[DATA_LENGTH]		= (unsigned char)2;
			instruction_packet[DATA_LENGTH + 1] = this->getChecksum(instruction_packet);

			response = this->Read(instruction_packet, 8);

			printf("Response: %d\n", response.message);
			break;
		case WRITE:
			instruction_packet[LENGTH]			= (unsigned char)SINGLE_ARGUMENT_LENGTH;
			instruction_packet[INSTRUCTION] 	= (unsigned char)WRITE_INSTRUCTION;
			instruction_packet[VALUE] 			= (unsigned char)value;
			instruction_packet[VALUE + 1] 		= this->getChecksum(instruction_packet);

			response.length 					= write(this->USB, instruction_packet, 8); 

			printf("Geschreven: %d\n", response.length);

			break;
		case WRITE_PAIR:
			instruction_packet[LENGTH]			= (unsigned char)DOUBLE_ARGUMENT_LENGTH;
			instruction_packet[INSTRUCTION]		= (unsigned char)WRITE_INSTRUCTION;

			unsigned char lowByte 				= (unsigned char)(value & 0xff);
			unsigned char highByte				= (unsigned char)((value & 0xff00) >> 8); 

			instruction_packet[LOWBYTE] 		= lowByte;
			instruction_packet[HIGHBYTE]		= highByte;

			instruction_packet[HIGHBYTE + 1] 	= this->getChecksum(instruction_packet);

			response.length 					= write(this->USB, instruction_packet, 9);
			break;
	}

	return response;
}

CM730Serial::Response CM730Serial::Read(unsigned char* instruction_packet, int packet_length) {
	unsigned char status_packet[8];

	Response response;

	int lowByte, highByte;
	unsigned short word;

	int result = write(this->USB, instruction_packet, packet_length);

	printf("Bits: %d\n", result);

	//usleep(100000);

	result = read(this->USB, status_packet, 8);

	printf("Ontvangen: %d\n", result);

	if(instruction_packet[DATA_LENGTH] == 2) {
		lowByte 	= (int)status_packet[ADDRESS];
		highByte 	= (int)status_packet[VALUE];

		word = highByte;
		word = word << 8;
		word = word + lowByte;

		response.message = (int)word;
	}
	else response.message = (int)status_packet[ADDRESS];

	response.length = result;

	return response;
}


unsigned char CM730Serial::getChecksum(unsigned char* packet) {
	unsigned char checksum = 0;
	for(int i = ID; i < (ID + packet[LENGTH] + 1); i++) {
		checksum += packet[i];
	}

	return ~checksum;
}