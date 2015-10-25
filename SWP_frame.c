#include "SWP_frame.h"

char* serializeFrame(FRAME frame)
{
    char* sFrame = (char*) malloc(FRAMESIZE * sizeof(char));
    sFrame[0] = frame.soh;
    sFrame[1] = frame.frameno;
    sFrame[2] = frame.stx;
    sprintf(sFrame, "%s%s", sFrame, frame.data);
    sprintf(sFrame, "%s%c", sFrame, frame.etx);
    return sFrame;
}

void setFrame(FRAME *frame, char *data, int frameno)
{
    int i;

    frame->soh = SOH;
    frame->frameno = frameno;
    frame->stx = STX;
    for (i = 0; i < DATASIZE; ++i) {
        frame->data[i] = data[i];
    }
    frame->etx = ETX;
    frame->checksum = calc_crc16(serializeFrame(*frame));
}

void transferFrame(FRAME *frameout, FRAME framein)
{
  frameout->soh = framein.soh;
  frameout->frameno = framein.frameno;
  frameout->stx = framein.stx;
  int i;
  for (i = 0; i < DATASIZE; ++i) {
      frameout->data[i] = framein.data[i];
  }
  frameout->etx = framein.etx;
  frameout->checksum = framein.checksum;
}

unsigned int compareChecksum(FRAME frame)
{
	return calc_crc16(serializeFrame(frame)) == frame.checksum ? ACK : NAK;
}

unsigned short calc_crc16(char *buf)
{
    unsigned short crc = 0xFFFF;
    unsigned short i;

    for (i=0; buf[i] != 0; i++) {
        crc = ((crc << 8) & 0xFFFF) ^ crctab[(crc >> 8) ^ (buf[i] & 0xFF)];
    }

    return crc;
}

int isValidFrame(FRAME frame)
{
  return (frame.soh == SOH && frame.stx == STX && frame.etx == ETX);
}

char* getDataFrame(FRAME frame)
{
  char* str = (char*) malloc((DATASIZE) * sizeof(char));
  sprintf(str, "%s", frame.data);
  return str;
}

int isValidChar(char inChar){
  return (inChar >= 32 || inChar == CR || inChar == Endfile || inChar == LF);
}
