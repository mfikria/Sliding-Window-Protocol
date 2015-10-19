#include <stdio.h>

void stringToShort(unsigned char* pchar, unsigned short* pshort, int offset)
{
  *pshort = ((*(pchar+offset+1) << 8) & 0xFF00) | (*(pchar+offset) & 0x00FF);
}

void shortToString(unsigned char* pchar, unsigned short* pshort, int offset)
{
  pchar[offset] = (*(pshort) & 0xFF);
  pchar[offset+1] = ((*pshort >> 8) & 0xFF);
}
