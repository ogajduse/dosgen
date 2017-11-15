#ifndef _HTTPFLOOD_H
#define _HTTPFLOOD_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


struct httpFloodParams{
	int id;	
	struct sockaddr_in *serveraddr;
	char *sendBuf;
	int threads_nbr;
        int duration;
        int read_time;

};

class httpFlood {
private:
	const char *dstIpStr;
	u_int16_t dstPort;
	char *path;
	char sendBuf[1024];
	struct sockaddr_in serveraddr;
	int threads_nbr;  
        int duration = 600;
        int read_time;
        
	static void* httpFloodThread(void *threadid);

public:
	httpFlood(int argc, char **argv);
	void run();
	
};

#endif
