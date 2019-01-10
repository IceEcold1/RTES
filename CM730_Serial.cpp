#include "CM730_Serial.h"

CM730Serial::CM730Serial() {
	this->READ.id 			= read1;	
	this->READ.length 		= 4;
	this->READ.action 		= 2;
	this->READ.value 		= 2;

	this->WRITE.id 			= write1;
	this->WRITE.length 		= 5;
	this->WRITE.action 		= 3;
	this->WRITE.value 		= 0;

	this->WRITE_PAIR.id 	= write_pair1;
	this->WRITE_PAIR.length = 5;
	this->WRITE_PAIR.action = 3;
	this->WRITE_PAIR.value 	= 0;

	this->USB = open("/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NONBLOCK);
	
	cfsetospeed(&this->tty, (speed_t)baudrate);
	cfsetispeed(&this->tty, (speed_t)baudrate);
	
	this->tty.c_cflag      = B38400|CS8|CLOCAL|CREAD;
	this->tty.c_iflag      = IGNPAR;
	this->tty.c_oflag      = 0;
	this->tty.c_lflag      = 0;
	this->tty.c_cc[VTIME]  = 0;
	this->tty.c_cc[VMIN]   = 0;

	ioctl(USB, TIOCGSERIAL, &serinfo);

	this->serinfo.flags &= ~ASYNC_SPD_MASK;
    this->serinfo.flags |= ASYNC_SPD_CUST;
    this->serinfo.custom_divisor = serinfo.baud_base / baudrate;

   	ioctl(this->USB, TIOCSSERIAL, &this->serinfo);
	
	cfmakeraw(&this->tty);
	
	tcflush(this->USB, TCIFLUSH);
	
	tcsetattr(this->USB, TCSANOW, &this->tty);
}

int CM730Serial::action(Method method, int id, int address, int value) {
	unsigned char packet[256 + 10] 	= {0, };
	
	packet[0] = 0xFF;
	packet[1] = 0xFF;
	
	packet[ID] 		= (unsigned char)id;
	packet[LENGTH] 	= method.length;
	packet[ADDRESS] = (unsigned char)address;
	packet[ACTION]	= (unsigned char)method.action;
	
	unsigned char checksum = this->getChecksum(packet);
	
	int length = method.length + 4;
	packet[length - 1] = checksum;
	
	switch(method.id) {
		case write1:
			this->Write(packet, value);
			break;
		case write_pair1:
			this->WritePair(packet, value);
			break;
		case read1:
			this->Read(packet);
			break;
	}
	return 0;
}

int CM730Serial::Write(unsigned char* packet, int value) {
	packet[VALUE] = (unsigned char)value;
	
	return write(this->USB, packet, (WRITE.length + 4));
}

int CM730Serial::WritePair(unsigned char* packet, int value) {
	unsigned char lowByte 	= (unsigned char)(value & 0xff);
	unsigned char highByte	= (unsigned char)((value & 0xff00) >> 8); 
	
	packet[LOWBYTE] 	= lowByte; 
	packet[HIGHBYTE] 	= highByte;
	
	return write(this->USB, packet, (WRITE.length + 4));
}

int CM730Serial::Read(unsigned char* packet) {	
	packet[VALUE] = READ.value;
	
	return read(this->USB, packet, (WRITE.length + 4));
}

unsigned char CM730Serial::getChecksum(unsigned char* packet) {
	unsigned char checksum = 0x00;
	
	for(int i = 2; i < packet[3] + 3; i++) checksum += packet[i];
	
	return ~checksum;
}