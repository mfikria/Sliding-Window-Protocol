#include "SWP_ack.h"
#include "math.h"

void setAck(ACKN *ackn, FRAME frame)
{
	unsigned short cs = calc_crc16(serializeFrame(frame));
	/*
	if(cs == frame.checksum)
	{
		ackn->ack = ACK;
	}
	else
	{
		ackn->ack = NAK;
	}*/

	int random = rand() % 3;
	if(random == 0){
		ackn->ack = ACK;
	}
	else if(random == 1){
		ackn->ack = NAK;
	}
	else if(random == 2){

	}

	ackn->frameno = frame.frameno;
	ackn->checksum = cs;
}

void printAck(ACKN ack){
	printf("%d %d %hu\n", ack.ack, ack.frameno, ack.checksum );
}
