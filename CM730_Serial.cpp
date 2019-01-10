#include "CM730_Serial.h"

CM730Serial::CM730Serial() {
	double baudrate = 1000000.0;

	struct termios tty;
	struct serial_struct serinfo;
	
	// Seriele communicatie opzetten.
	this->USB = open("/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NONBLOCK);
	
	// Baudrate instellen.
	cfsetospeed(&tty, (speed_t)baudrate);
	cfsetispeed(&tty, (speed_t)baudrate);
	
	// Gegevens initaliseren die nodig zijn voor de communicatie.
	tty.c_cflag      = B38400|CS8|CLOCAL|CREAD;
	tty.c_iflag      = IGNPAR;
	tty.c_oflag      = 0;
	tty.c_lflag      = 0;
	tty.c_cc[VTIME]  = 0;
	tty.c_cc[VMIN]   = 0;
	
	/* Wordt gebruikt zodat commando's verstuurd kunnen worden */
	ioctl(USB, TIOCGSERIAL, &serinfo);

	/* Flags initaliseren */
	serinfo.flags &= ~ASYNC_SPD_MASK;
    serinfo.flags |= ASYNC_SPD_CUST;
    serinfo.custom_divisor = serinfo.baud_base / baudrate;

   	ioctl(USB, TIOCSSERIAL, &serinfo);
	
	cfmakeraw(&tty);
	
	/* Gegevens weggooien die nooit zijn gebruikt */
	tcflush(this->USB, TCIFLUSH);
	
	/* Termios gegevens klaarzetten */
	tcsetattr(this->USB, TCSANOW, &tty);
}

CM730Serial::sub_cont_response CM730Serial::action(Methods method, int id, int address, int value) {
	/* Pakket opstellen die opgestuurd moet worden naar de sub-controller */
	unsigned char txpacket[256 + 10] = {0, };
	
	/* Struct waarin de ontvangen gegevens worden opgeslagen. */
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
	
	/*unsigned char lowByte 	= (unsigned char)(value & 0xff);
	unsigned char highByte	= (unsigned char)((value & 0xff00) >> 8); 

	txpacket[LOWBYTE] = lowByte;
	txpacket[HIGHBYTE] = highByte; */
	
	unsigned char checksum = this->getChecksum(txpacket);
	
	int length = txpacket[LENGTH] + 4; 
	txpacket[length - 1] = checksum;

	switch(method) {
		case WRITE:
			response.length = this->Write(txpacket, value, length);
			return response;
			break;
		case WRITE_PAIR:
			response.length = this->WritePair(txpacket, value, length);
			return response;
			break;
		case READ:
			response = this->Read(txpacket, length);
			return response;
			break;
	}

	/*-1 is an error*/
	response.length = -1;
	return response;
}
/* Schrijven naar één adres */
int CM730Serial::Write(unsigned char* packet, int value, int length) {
	packet[VALUE] = (unsigned char)value;

	printf("packet: %s\n", packet);
	
	return write(this->USB, packet, length);
}
/* Schrijven naar 2 addressen: Low en High byte die gebruikt worden in de sub-controller */
int CM730Serial::WritePair(unsigned char* packet, int value, int length) {

	unsigned char lowByte 	= (unsigned char)(value & 0xff);
	unsigned char highByte	= (unsigned char)((value & 0xff00) >> 8); 

	packet[LOWBYTE] = lowByte;
	packet[HIGHBYTE] = highByte;
	
	return written = write(this->USB, packet, length);

	return written;
}

/* Lezen uit 2 adressen. Low en high byte. */
CM730Serial::sub_cont_response CM730Serial::Read(unsigned char* packet, int length) {	
	packet[VALUE] = READ.value;
	/* Vraag aan de sub-controller om gegevens op te sturen */
	write(this->USB, packet, length);
	
	sub_cont_response response;
	
	char[100] readMessage;

	int to_length = packet[VALUE] + 6;
	
	/* Lees de gegevens */
	response.length = read(this->USB, readMessage, sizeof readMessage);
	response.message = readMessage;

	return response;
}

/* Checksum aanmaken die meegestuurd worden in de commando */
unsigned char CM730Serial::getChecksum(unsigned char* packet) {
	unsigned char checksum = 0x00;
	
	for(int i = 2; i < packet[LENGTH] + 3; i++) checksum += packet[i];
	
	return ~checksum;
}