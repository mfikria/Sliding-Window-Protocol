CC=gcc
CFLAGS = -g
# uncomment this for SunOS
# LIBS = -lsocket -lnsl

all: clean transmitter receiver

transmitter: transmitter.c
	$(CC) -pthread -o transmitter transmitter.c SWP_frame.c $(LIBS)

receiver: receiver.c
	$(CC) -pthread -o receiver receiver.c SWP_frame.c SWP_ack.c $(LIBS)

clean:
	rm -f transmitter receiver transmitter.o receiver.o
