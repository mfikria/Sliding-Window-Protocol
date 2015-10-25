#ifndef _SWP_FRAME_H_
#define _SWP_FRAME_H_

#include "dcomm.h"
#include <string.h>
#define POLY 0x8408

typedef struct FRAMES
{
	unsigned int soh;					// start of header
	Byte frameno;							// frame number
	unsigned int stx;					// start of text
	Byte data[DATASIZE];			// data
	unsigned int etx;					// end of text
	unsigned short checksum;	// checksum
} FRAME;

typedef struct FRAMEBUFS{
	FRAME frame;
	int status;
	/*
		status of frame:
		in transmitter: 2 -> sending frame success (get ACK)
                    1 -> frame has been buffered and ready to be sent
										0 -> frame have not been sent OR error while sending frame (get NAK)
		in receiver		: 1 -> frame has been delivered in receiver
										0 -> frame has not been delivered yet
	*/
} FRAMEBUF;

char* serializeFrame(FRAME frame);
/*
	Create a string from data frame, soh + frameno + stx + data + etx
*/

void setFrame(FRAME *frame, char *data, int frameno);
/*
	Assign frame's data and attributes
*/

void transferFrame(FRAME *frameout, FRAME framein);
/*
	Transfer frame's data and attributes from one frame to another
*/

unsigned int compareChecksum(FRAME frame);
/*
	compare checksum value from serializing frame's data and frame's attribute
*/

unsigned short calc_crc16(char *data_p);
/*
	Get frame's checksum value using CRC 16 method
*/

int isValidFrame(FRAME frame);
/*
  Check whether frame is in valid format or not
*/

char* getDataFrame(FRAME frame);
/*
  get data frame
*/

int isValidChar(char inChar);
/*
Check whether character is  a valid character? not ( >32, CR, LF atau end-of-file)
*/
#endif
