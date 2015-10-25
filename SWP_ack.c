#include "SWP_ack.h"

void setAck(ACKN *ack, FRAME frame)
{
	ack->ack = compareChecksum(frame);
	ack->frameno = frame.frameno;
	ack->checksum = calc_crc16(serializeFrame(frame));
}
