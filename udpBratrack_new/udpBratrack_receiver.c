/*
	Author: Jeronimo Grandi 
	modified from Paul Krzyzanowski
	and merged with Franscico Pinto udpClass
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include "BraTrackUDP.h"


using namespace std;
using namespace BraTrackUDP;

classInputProtocol udpReceiver;
vector<type_artifact> list_of_artifacts;

unsigned short int SERVICE_PORT;
unsigned int BUFFER_SIZE;

void *DataReceiver(void*)
{
	unsigned long long int time_stamp;

	for(;;)
	{
		udpReceiver.receive_frame_not_blocking(time_stamp,list_of_artifacts);
		cout << "timestamp: " << time_stamp <<endl;
		cout << "ID: " << list_of_artifacts[0].id << endl;
		for (int i = 0; i < 12; i++)
			cout << list_of_artifacts[0].transform[i] << endl;
			
	}	

}


int main(int argc, char **argv)
{
	BUFFER_SIZE = 1024;
	if (argc < 2)
	{
		printf("No port specified, running on port 3000 (default).\n");
		SERVICE_PORT = 3000;
	}
	else
		SERVICE_PORT = atoi(argv[1]);


	if(udpReceiver.init(SERVICE_PORT,BUFFER_SIZE))
		return 0;
	udpReceiver.set_package_limits(4); //maximum number of artifacts
	type_artifact art1;
	art1.id = "1";

	list_of_artifacts.push_back(art1);

	pthread_t thr;
	pthread_create(&thr, NULL, DataReceiver,NULL); //initialize thread

	//main thread

	pthread_join(thr,NULL); //join thread

	return 0;
}
