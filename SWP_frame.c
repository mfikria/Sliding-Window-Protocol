#include "SWP_frame.h"

char* serializeFrame(FRAME frame)
{
    char* sFrame = (char*) malloc((DATASIZE+4)*sizeof(char));
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
    for (i = 0; i < DATASIZE; ++i)
    {
      frame->data[i] = data[i];
    }
    frame->etx = ETX;
    frame->checksum = crc16(serializeFrame(*frame));
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
	return crc16(serializeFrame(frame)) == frame.checksum ? ACK : NAK;
}

unsigned short crc16(char *data_p)
{
      unsigned int data,
                   crc = 0xffff;
      int length = 4 + DATASIZE;

      if (length == 0) return (~crc);

      do
      {
        int i;
        for (i=0, data=(unsigned int)*data_p++ & 0xff; i < 8; i++, data >>= 1)
        {
            if ((crc & 0x0001) ^ (data & 0x0001)) crc = (crc >> 1) ^ BITPATTERN;
            else  crc >>= 1;
        }
      } while (--length);

      crc = ~crc;
      data = crc;

      return (crc << 8) | (data >> 8 & 0xff);
}

int isValidFrame(FRAME frame)
{
  return (frame.soh == SOH && frame.stx == STX && frame.etx == ETX);
}

char* getDataFrame(FRAME frame)
{
  char* str = (char*) malloc((DATASIZE) * sizeof(char));
  //sprintf(str, "%s", frame.data);
  int i;
  for(i = 0; i < DATASIZE; i++)
  {
    str[i] = frame.data[i];
  }
  return str;
}

int isValidChar(char inChar){
  return (inChar >= 32 || inChar == CR || inChar == Endfile || inChar == LF);
}

void printFrame (FRAME frame)
{
  printf("\n%d\n%d\n%d\n%s\n%d\n%hu\n", frame.soh, frame.frameno, frame.stx, getDataFrame(frame), frame.etx, frame.checksum);
}
