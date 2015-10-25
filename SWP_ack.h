#ifndef _SWP_ACK_H_
#define _SWP_ACK_H_

#include "dcomm.h"
#include "SWP_frame.h"
#include "qframe.h"

typedef struct
{
	unsigned int ack;					// Acknowledgement
	Byte frameno;							// frame number
	unsigned short checksum;	// checksum
} ACKN;

void setAck(ACKN *ack, FRAME frame);

#endif
