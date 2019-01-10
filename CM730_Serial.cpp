#include "CM730_Serial.h"

CM730Serial::CM730Serial() {
	double baudrate = 1000000.0;

	struct termios tty;
	struct serial_struct serinfo;
	
	/*Open serial port*/
	this->USB = open("/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NONBLOCK);
	
	/*Set baudrate*/
	cfsetospeed(&tty, (speed_t)baudrate);
	cfsetispeed(&tty, (speed_t)baudrate);
	
	/*Initialise serial flags and config*/
	tty.c_cflag      = B38400|CS8|CLOCAL|CREAD;
	tty.c_iflag      = IGNPAR;
	tty.c_oflag      = 0;
	tty.c_lflag      = 0;
	tty.c_cc[VTIME]  = 0;
	tty.c_cc[VMIN]   = 0;
	
	/*Is used to send commands*/
	ioctl(USB, TIOCGSERIAL, &serinfo);

	/*Initialise flags*/
	serinfo.flags &= ~ASYNC_SPD_MASK;
    serinfo.flags |= ASYNC_SPD_CUST;
    serinfo.custom_divisor = serinfo.baud_base / baudrate;

   	ioctl(USB, TIOCSSERIAL, &serinfo);
	
	cfmakeraw(&tty);
	
	/*Flush non-read data on the CM730*/
	tcflush(this->USB, TCIFLUSH);
	
	/*Initialise termios data*/
	tcsetattr(this->USB, TCSANOW, &tty);
}

CM730Serial::sub_cont_response CM730Serial::action(enum_methods method, int id, int address, int value) {
	/*Declare package to be sent to the sub-controller*/
	unsigned char txpacket[256 + 10] = {0, };
	
	/*Declare the struct in which data is saved*/
	sub_cont_response response;

	txpacket[0] = 0xFF;
	txpacket[1] = 0xFF;

	txpacket[ID] = (unsigned char)id;
	txpacket[ADDRESS] = (unsigned char)address;
	
	if(method == READ) {
		txpacket[LENGTH] = READ_LENGTH;	
		txpacket[ACTION] = (unsigned char)READ_ACTION;
	}
	else {
		txpacket[LENGTH] = WRITE_LENGTH;	
		txpacket[ACTION] = (unsigned char)WRITE_ACTION;
	}
	
	unsigned char checksum = this->get_checksum(txpacket);
	
	int length = txpacket[LENGTH] + 4; 
	txpacket[length - 1] = checksum;

	switch(method) {
		case WRITE:
			response.length = this->write_cm730(txpacket, value, length);
			return response;
			break;
		case WRITE_PAIR:
			response.length = this->write_pair_cm730(txpacket, value, length);
			return response;
			break;
		case READ:
			response = this->read_cm730(txpacket, length);
			return response;
			break;
	}

	/*-1 is an error*/
	response.length = -1;
	return response;
}
/*Write a single data package to a single address*/
int CM730Serial::write_cm730(unsigned char* packet, int value, int length) {
	packet[VALUE] = (unsigned char)value;

	printf("packet: %s\n", packet);
	
	return write(this->USB, packet, length);
}
/*Write a single data package to multiple adresses, AKA the high and low byte*/
int CM730Serial::write_pair_cm730(unsigned char* packet, int value, int length) {

	unsigned char lowByte 	= (unsigned char)(value & 0xff);
	unsigned char highByte	= (unsigned char)((value & 0xff00) >> 8); 

	packet[LOWBYTE] = lowByte;
	packet[HIGHBYTE] = highByte;
	
	return write(this->USB, packet, length);
}

/*Read from two addresses, AKA the high and low byte*/
CM730Serial::sub_cont_response CM730Serial::read_cm730(unsigned char* packet, int length) {	

	packet[VALUE] = READ_VALUE;
	/*Ask the sub controller to initialise data to be read*/
	int ready = write(this->USB, packet, length);
	
	sub_cont_response response;
	
	char readMessage[100];


	//int to_length = packet[VALUE] + 6;
	
	if(ready > -1)
	{
		/*Read the data and put in struct*/
		response.length = read(this->USB, readMessage, sizeof readMessage);
		memcpy(response.message, readMessage, response.length);
	}
	else
	{
		response.length = -1;
	}

	return response;
}

/*Checksum to be passed to the sub-controller to check integrity.*/
unsigned char CM730Serial::get_checksum(unsigned char* packet) {
	unsigned char checksum = 0x00;
	
	for(int i = 2; i < packet[LENGTH] + 3; i++) checksum += packet[i];
	
	return ~checksum;
}