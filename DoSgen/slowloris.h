#ifndef SLOWLORIS_H
#define SLOWLORIS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

struct SlowLorisParams{
	int id;
	int con_nbr;
	SOCKADDR_IN servaddr;
	int timeout;
        int duration;
} ;


class SlowLoris {
    private:
	char *target = NULL;
	int port = 80;
	int timeout = 15;
	int con_nbr = 300;
	int duration = 600;	
	
	SOCKADDR_IN servaddr = { 0 };
	int *pids;

	
	static void* slowLorisThread(void *threadid);

    public:
	SlowLoris(int argc, char **argv);
	void run();
	
};


#endif
