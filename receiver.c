/*
* File : receiver.cpp
*/
#include "SWP_ack.h"
#include "SWP_frame.h"

/* Socket */
int sockfd; // listen on sock_fd
struct sockaddr_in thisAddr,	/* our address */
			remAddr;	/* remote address */
socklen_t remAddrLen = sizeof(remAddr);		/* length of addresses */

/* Global Variables */
static int countRcvdFrames = 0,   // number of Received Bytes
					countCnsmdFrames = 0;   // number of Consumed Bytes

int servicePort;

FRAMEBUF framebuf[BUFSIZE];
slidingWindow window ={0, WINSIZE - 1};

int main(int argc, char *argv[]){

  //check whether argument is valid or not
	if (argc != 2) {
		printf("./receiver <Service Port>\n");
		return 0;
	}

	// define the service port
	servicePort = atoi(argv[1]);

  // create connection with UDP socket
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("cannot create socket\n");
    return 0;
  }

  // bind the socket to a specific port and IP address
  memset((char *)&thisAddr, 0, sizeof(thisAddr));
  thisAddr.sin_family = AF_INET;
  thisAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  thisAddr.sin_port = htons(servicePort);

  if (bind(sockfd, (struct sockaddr *)&thisAddr, sizeof(thisAddr)) < 0) {
    perror("bind failed");
    return 0;
  }
	else {
		printf("Binding pada 127.0.0.1:%d ...\n",servicePort);
	}

  while (1) {
		FRAME frame;
		if(recvfrom(sockfd, &frame, FRAMESIZE, 0, (struct sockaddr *)&remAddr, &remAddrLen) > 0)
		{
			ACKN ackn;
			setAck(&ackn, frame);
			if(isValidFrame(frame))
			{
				transferFrame(&framebuf[frame.frameno].frame, frame);
				framebuf[frame.frameno].status = 1;
				printf("Frame-%d received: %s\n", frame.frameno, getDataFrame(frame));
			}

			if(sendto(sockfd, &ackn, ACKNSIZE, 0, (struct sockaddr *)&remAddr, remAddrLen) < 0)
			{
				perror("Failed to Send.");
				return 0;
			}
			if(framebuf[window.head].status == 1)
			{
				int numMove = 0;
				int j;
				for(j = window.head; (j <= window.tail) && (framebuf[j].status == 1); j++)
				{
					numMove++;
				}
				window.head += numMove;
				window.tail += numMove;
			}
		}
  }
	return 0;
}
