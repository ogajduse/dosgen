#ifndef RUDY_H
#define RUDY_H

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

struct RudyParams{
	int id;
	int con_nbr;
	SOCKADDR_IN servaddr;
	int timeout, duration;
	char *target;
	char *path;
	char *param;
} ;

class Rudy {
	private:
            int port = 80;
            int timeout = 15;
            int con_nbr = 300;
            int test = 0; // used as boolean
            int duration = 600;	
            char *target;
            char *path;
            char *param;

            SOCKADDR_IN servaddr = { 0 };
            int *pids;


            static void* RudyThread(void *threadid);

	public:       
            Rudy(int argc, char **argv);
            void run();
};

#endif


