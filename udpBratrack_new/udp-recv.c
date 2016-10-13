/*
	modified by Jeronimo Grandi 
	from Paul Krzyzanowski
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;
#define BUFSIZE 1024

struct sockaddr_in myaddr;	/* our address */
struct sockaddr_in remaddr;	/* remote address */
socklen_t addrlen = sizeof(remaddr);		/* length of addresses */
int recvlen;			/* # bytes received */
int fd;				/* our socket */
unsigned char buf[BUFSIZE];	/* receive buffer */
//char *buffer;

int SERVICE_PORT;


double transform[12];

void *DataReceiver(void*)
{
	/* now loop, receiving data and printing what we received */
	for (;;) 
	{
		printf("waiting on port %d\n", SERVICE_PORT);
		recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
		printf("received %d bytes\n", recvlen);
		if (recvlen > 0) {

			void *cursor = buf;
			unsigned long long int timestamp;
			timestamp = *((unsigned long long int*)cursor);
			cursor = (unsigned char*)cursor + sizeof(unsigned long long int);
			char *id = (char*)cursor;
			cursor = (unsigned char*)cursor + 8*sizeof(char);

			printf("%d\n",timestamp );
			printf("id: \"%s\"\n", id);

			for(unsigned int i=0; i<12; i++){
				transform[i] = *((double*)cursor);
				cursor = (unsigned char*)cursor + sizeof(double);
				printf("\"%f\"\n", transform[i]);

			}

		}
		else
			printf("No data received\n");
	}

}


int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("No port specified, running on port 3000 (default).\n");
		SERVICE_PORT = 3000;
	}
	else
		SERVICE_PORT = atoi(argv[1]);

	/* create a UDP socket */

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket\n");
		return 0;
	}

	/* bind the socket to any valid IP address and a specific port */

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(SERVICE_PORT);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}

	pthread_t thr;
	pthread_create(&thr, NULL, DataReceiver,NULL); //initialize thread

	//main thread

	pthread_join(thr,NULL); //join thread

	/* never exits */
}
