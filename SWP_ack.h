#ifndef _SWP_ACK_H_
#define _SWP_ACK_H_

#include "SWP_frame.h"

typedef struct
{
	unsigned int ack;
	Byte frameno;
	unsigned short checksum;
} ACK;

unsigned int compareChecksum(FRAME frame) {
	return calc_crc16(dataFrameToString(frame)) == frame.checksum ? ACK : NAK;
}

void createAck(ACK *ack, FRAME frame){
	ack->ack = compareChecksum(frame);
	ack->frameno = frame.frameno;
	ack->checksum = calc_crc16(dataFrameToString(frame));
}

char* ackToString(ACK ack){
    char* strack = (char*) malloc(ACKSIZE * sizeof(char));
    strack[0] = ack.ack;
    strack[1] = ack.frameno;
    strack[2] = ack.checksum;
		sprintf(strack, "%s%x", strack, frame.checksum);
    return strack;
}

#endif
