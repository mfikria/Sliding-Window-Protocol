
/*
* File : dcomm.h
*/

#ifndef _DCOMM_H_
#define _DCOMM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

/* ASCII Const */
#define SOH 1 /* Start of Header Character */
#define STX 2 /*Start of Text Character */
#define ETX 3 /*End of Text Character */
#define ENQ 5 /*Enquiry Character */
#define ACK 6 /*Acknowledgement */
#define BEL 7 /*Message Error Warning */
#define CR 13 /*Carriage Return */
#define LF 10 /*Line Feed */
#define NAK 21 /*Negative Acknowledgement */
#define Endfile 26 /*End of file character */
#define ESC 27 /*ESC key */

/* Const */
#define BYTESIZE 256 	/* The maximum value of a byte */
#define MAXLEN 1024		/* Maximum messages length */
#define WINSIZE 5			/* Window size */
#define DATASIZE 5		/* data frame size */
#define FRAMESIZE 13	/* data frame size */
#define ACKNSIZE 6		/* Acknowledgement size */
#define QFRAMESIZE 15 /* Size of Circular Queue */
#define BUFSIZE 255		/* Size of frame buffer */
#define DELAY 500			/* Delay to adjust speed in milliseconds */
#define BITPATTERN 0x8408

typedef unsigned char Byte;

typedef struct sw {
	int head;	// indicate index of first window's frame in frame buffer
	int tail;	// indicate index of last window's frame in frame buffer
} slidingWindow;

#endif
