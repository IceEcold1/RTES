#include "CM730_Serial.h"

bool CM730Serial::CM730() {
	this->USB = open("/dev/ttyUSB0, O_RDWR|O_NOCTTY|O_NONBLOCK");
	
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

int CM730Serial::action(TYPE type, int id, int address, int value) {
	unsigned char packet[256 + 10] 	= {0, };
	
	packet[0] = 0xFF;
	packet[1] = 0xFF;
	
	packet[packetItem.ID] 		= (unsigned char)id;
	packet[packetItem.LENGTH] 	= type.Length;
	packet[packetItem.ADDRESS] 	= (unsigned char)address;
	packet[packetItem.ACTION]	= (unsigned char)type.Action;
	
	unsigned char checksum = this->getChecksum(packet);
	
	int length = type.Length + 4;
	packet[length - 1] = checksum;
	
	switch(type) {
		write:
			return this->write(packet, value);
			break;
		write_pair:
			return this->writePair(packet, value);
			break;
		read:
			return this->read(packet);
			break;
	}
}

int CM730Serial::write(unsigned char packet, int value) {
	packet[packetItem.VALUE] = (unsigned char)value;
	
	return write(this->USB, packet, (WRITE.Length + 4));
}

int CM730Serial::writePair(unsigned char packet, int value) {
	unsigned char lowByte 	= (unsigned char)(value & 0xff);
	unsigned char highByte	= (unsigned char)((value & 0xff00) >> 8); 
	
	packet[packetItem.LOWBYTE] 		= lowByte; 
	packet[packetItem.HIGHBYTE] 	= highByte;
	
	return write(this->USB, packet, (WRITE.Length + 4));
}

int CM730Serial::read(unsigned char packet) {	
	int length = READ.Length + 4;
	
	packet[packetItem.VALUE] = READ.Value;
	
	return read(this->USB, packet, (WRITE.Length + 4));
}

unsigned char getChecksum(unsigned char packet) {
	unsigned char checksum = 0x00;
	
	for(int i = 2; i < packet[3] + 3; i++) checksum += packet[i];
	
	return ~checksum;
}