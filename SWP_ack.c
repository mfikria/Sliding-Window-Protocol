#include "SWP_ack.h"

void setAck(ACKN *ackn, FRAME frame)
{
	unsigned short cs = calc_crc16(serializeFrame(frame));
	if(cs == frame.checksum)
	{
		ackn->ack = ACK;
	}
	else
	{
		ackn->ack = NAK;
	}
	ackn->frameno = frame.frameno;
	ackn->checksum = cs;
}

void printAck(ACKN ack){
	printf("%d %d %hu\n", ack.ack, ack.frameno, ack.checksum );
}
