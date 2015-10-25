#ifndef _SWP_ACK_H_
#define _SWP_ACK_H_

#include "dcomm.h"
#include "SWP_frame.h"

typedef struct
{
	unsigned int ack;					// Acknowledgement
	Byte frameno;							// frame number
	unsigned short checksum;	// checksum
} ACKN;

void setAck(ACKN *ack, FRAME frame);
void printAck(ACKN ack);
#endif
