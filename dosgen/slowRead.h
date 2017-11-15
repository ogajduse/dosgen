#ifndef _SLOWREAD_H
#define _SLOWREAD_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

struct SlowReadParams{
	int id;	
	//struct sockaddr_in servaddr;
	SOCKADDR_IN servaddr;
        char *sendBuf;
	int duration;
	int interval;
	int buff_size;
	int window_size;
} ;


class SlowRead {
private:

	const char *dstIpStr;
	u_int16_t dstPort;
	char *path;
	unsigned int connections = 100;
	int duration;
	int window_size;
	int buff_size;
	int interval;
        
	char sendBuf[1024];
	//struct sockaddr_in servaddr;
	SOCKADDR_IN servaddr = { 0 };
	
        int con_nbr;


	
	static void* slowReadThread(void *threadid);

public:
	SlowRead(int argc, char **argv);
	void run();
	
};

#endif
