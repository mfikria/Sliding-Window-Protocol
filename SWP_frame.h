#ifndef _SWP_FRAME_H_
#define _SWP_FRAME_H_

#include "dcomm.h"
#include "crc16.h"
#include <string.h>

typedef struct FRAME
{
	unsigned int soh;
	Byte frameno;
	unsigned int stx;
	Byte *data;
	unsigned int etx;
	unsigned short checksum;
} FRAME;

char* dataFrameToString(FRAME frame){
    char* tFrame = (char*) malloc((1+1+1+DATASIZE+1+4) * sizeof(char));
    tFrame[0] = frame.soh;
    tFrame[1] = frame.frameno;
    tFrame[2] = frame.stx;
    sprintf(tFrame, "%s%s", tFrame, frame.data);
    sprintf(tFrame, "%s%c", tFrame, frame.etx);
    return tFrame;
}

char* frameToString(FRAME frame) {
    char* tFrame = (char*) malloc((1+1+1+DATASIZE+1+4) * sizeof(char));
    sprintf(tFrame, "%s%x", dataFrameToString(frame), frame.checksum);
    return tFrame;
}

void setFrame(FRAME *frame, char *data, int frameno) {
    int i;

    frame->soh = SOH;
    frame->frameno = frameno;
    frame->stx = STX;
    frame->data = (Byte*) malloc(DATASIZE * sizeof(Byte));
    for (i = 0; i < DATASIZE; ++i) {
        frame->data[i] = data[i];
    }
    frame->etx = ETX;
    frame->checksum = calc_crc16(dataFrameToString(*frame));
}

void stringToFrame(FRAME *frame, char *dataFrame) {
    int i;

    frame->soh = dataFrame[0];
    frame->frameno = dataFrame[1];
    frame->stx = dataFrame[2];
    frame->data = (Byte*) malloc(DATASIZE * sizeof(Byte));
    for (i = 0; i < DATASIZE; ++i) {
        frame->data[i] = dataFrame[3+i];
    }
    frame->etx = dataFrame[3+DATASIZE];
    shortToString(&*dataFrame, &(frame->checksum), (3+DATASIZE+1));
}

#endif
