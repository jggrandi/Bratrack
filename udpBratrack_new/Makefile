
CC = g++
CFLAGS = -Wall 
CDEBUG = -g
COPTIMIZATION = -O4
WARNINGOFF = -w
PROG = UdpBratrackReceiver

SRC = udpBratrack_receiver.c BraTrackUDP.cpp
PTHREAD = -lpthread



$(PROG): $(SRC)
	 $(CC) $(CFLAGS) $(CDEBUG) -o $(PROG) $(SRC) $(PTHREAD)

clean:
	rm -f $(PROG)


all:
	make clean
	make $(PROG)