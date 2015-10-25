#include "dcomm.h"
#include "SWP_frame.h"
#include "SWP_ack.h"

/* Threads */
pthread_t tid[2];
pthread_mutex_t lock;
void* bufferingFrame(void* threadArgs);
void* transmitingFrame(void* threadArgs);

/* Making-Connection-related Variables */
const char *hostname;	/* change this to use a different hostname */
const char *fileInput;
int servicePort;
struct sockaddr_in remAddr;
int sockfd,
		remAddrLen = sizeof(remAddr);

FRAMEBUF framebuf[BUFSIZE];

/* Global Variables */
int countSendFrames  = 0,
		countBufFrames = 0, // frame number
		countRcvdAcks  = 0,
		isDone = 0;

FILE* file;

slidingWindow window ={0, WINSIZE - 1};

int main(int argc, char const *argv[]) {

	/* Assign hostname, servicePort, and input file */
	hostname = argv[1];
	servicePort = atoi(argv[2]);
	fileInput = argv[3];

	/* Create a Socket */
	if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
		perror("Socket Created");
		return 0;
	}
	else{
		printf("Membuat socket untuk koneksi %s : %d\n",hostname,servicePort);
	}

	/* Bind Socket */
	memset((char *) &remAddr, 0, sizeof(remAddr));
	remAddr.sin_family = AF_INET;
	remAddr.sin_port = htons(servicePort);
	if (inet_aton(hostname, &remAddr.sin_addr)==0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	/* Initialize mutex thread */
	if (pthread_mutex_init(&lock, NULL) != 0) {
    printf("\n mutex init failed\n");
    return 1;
	}

	pthread_create(&tid[0], NULL, bufferingFrame, NULL); // Thread for buffering data and set frame from file to buffef frame
	pthread_create(&tid[1], NULL, transmitingFrame, NULL); //

	ACKN ackn;
	while(1)
	{

		if(recvfrom(sockfd, &ackn, sizeof(ACKN), 0, (struct sockaddr *)&remAddr, &remAddrLen) > 0)
		{
			if(ackn.ack == ACK && ackn.checksum == calc_crc16(serializeFrame(framebuf[ackn.frameno].frame)))
			{
				printf("ACK received\n");
				framebuf[ackn.frameno].status = 2;
			}
			else if(ackn.ack == ACK && ackn.checksum != calc_crc16(serializeFrame(framebuf[ackn.frameno].frame))){
				printf("ACK error received\n");
			}
			else if(ackn.ack == NAK){
				printf("NAK received\n");
			}
			else
			{
				printf("ACK lost\n");
				/*
				if(sendto(sockfd, &framebuf[ackn.frameno].frame, sizeof(FRAME), 0, (struct sockaddr *)&remAddr, remAddrLen) < 0)
				{
					perror("Failed to Send.");
					return 0;
				}
				*/
			}
			
			if(framebuf[window.head].status == 2)
			{
				int numMove = 0;
				int j;
				for(j = window.head; (j <= window.tail) && (framebuf[j].status == 2); j++)
				{
					numMove++;
				}
				window.head += numMove;
				window.tail += numMove;
			}
			
			//usleep(DELAY* 1000);
		}
	}


	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	close(sockfd);
	pthread_mutex_destroy(&lock);

	return 0;
}

void* bufferingFrame(void* threadArgs)
{

	Byte buffer[DATASIZE];

	char ch;
	file = fopen(fileInput, "r");
	if(file == NULL){
		perror("File error\n");
		return 0;
	}

	int scanfile = fscanf(file, "%c", &ch);
	while(scanfile != EOF)
	{
		FRAME frame;
		pthread_mutex_lock(&lock);
		memset(buffer, 0, DATASIZE);
		int i;
		printf("Buffering frame-%d\n", countBufFrames);
		for(i = 0; (i < DATASIZE) && (scanfile != EOF); i++)
		{
			// buffering data
			buffer[i] = ch;
			scanfile = fscanf(file, "%c", &ch);
		}

		// Insert data from text and frame number to Frame
		setFrame(&frame, buffer, countBufFrames);

		// Buffering frame's data and attributes to frame buffer element
		framebuf[countBufFrames].status = 1;
		transferFrame(&framebuf[countBufFrames++].frame, frame);

		pthread_mutex_unlock(&lock);
		usleep(DELAY*1000);
	}
	if(scanfile == EOF)
	{
		isDone = 1;
		printf("EOF reached. Buffering frame done.\n");
		fclose(file);
	}
	return NULL;
}

void* transmitingFrame(void* threadArgs)
{

	int i, j;

	while(1)
	{
		pthread_mutex_lock(&lock);
		int k = window.head;
		printf("\n--------------------\n");
		for(i = k; i < k + WINSIZE; i++)
		{
			if(framebuf[i].status == 1)
			{
				printf("Sending frame-%d\n", i);
				if(sendto(sockfd, &framebuf[i].frame, sizeof(FRAME), 0, (struct sockaddr *)&remAddr, remAddrLen) < 0)
				{
					perror("Failed to Send.");
					return 0;
				}
			}
			usleep(DELAY*2000);
		}
		printf("--------------------\n\n");

		pthread_mutex_unlock(&lock);
		usleep(DELAY*WINSIZE*1000);
	}

	return NULL;
}
