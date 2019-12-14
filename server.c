/* Simple udp server */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <dirent.h>

#define BUFLEN 512	
#define PORT 8080 //porta para conexão
#define FILES_DIRECTORY "./storageFiles" // diretório onde os arquivos serão armazenados
#define DELIMITER "#@#"
#define SEPARATOR "/"
#define FILE_FOUND "00"
#define FILE_NOT_FOUND = "01"

void die (char *s)
{
	perror (s);
	exit (1);
}

void createStorageIfNotExists(){
	struct stat st = {0};
    if (stat(FILES_DIRECTORY, &st) == -1) {
        mkdir(FILES_DIRECTORY, 0700);
    }	
}


void getFileData(char *fileName, char **outFileBuffer, long *fSize){
	char *completeFilePath;
	completeFilePath = malloc(strlen(FILES_DIRECTORY) + strlen(fileName)+1 );
	strcpy(completeFilePath, FILES_DIRECTORY);
    strcat(completeFilePath, SEPARATOR);
    strcat(completeFilePath, fileName);

	FILE *f = fopen(completeFilePath, "rb");
    fseek(f, 0, SEEK_END);
    *fSize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
    char *buffer = malloc(*fSize + 1);
	*outFileBuffer = buffer;
    fread(buffer, 1, *fSize, f);
    fclose(f);
}


int existFileDirectory(char *fileName, char **outFileNameExt){
    int result = 0;
    struct stat st = {0};
    if (stat(FILES_DIRECTORY, &st) == -1) {
        mkdir(FILES_DIRECTORY, 0700);
    }

    DIR *d;
    struct dirent *dir;
    d = opendir(FILES_DIRECTORY);

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if(strstr(dir->d_name, fileName) != NULL){
                *outFileNameExt = dir->d_name;
                result = 1;
                break;
            }

            printf("Diretorio: %s Solicitado: %s\n", dir->d_name, fileName);
        }   
        closedir(d);
        return result;
    }
}


struct requestData {
	char *buffer; //bytes recebidos do client
	struct sockaddr_in client; //dados da requisição do client
	int *socket;
	int *slen;
};

void * handleRequest(void *ptr){
 	struct requestData * args = (struct requestData *)ptr;
	printf("%s", inet_ntoa(args->client.sin_addr));
	printf("%s",args->buffer);
    char * tmp = strtok(args->buffer, DELIMITER);
	char *verb = tmp;
	char * bufferData = strtok(NULL, DELIMITER);
	bufferData[strlen(bufferData)] = '\0';
	char * completeFileName;
	char * message;
	if(existFileDirectory(bufferData, &completeFileName)){
		printf("arquivo existe: %s\n", completeFileName);
		long fSize;
		char *bufferData;
		getFileData(completeFileName, &bufferData, &fSize);
		printf("Buffer para o client: %s\n", bufferData);

		char *response = malloc(strlen(bufferData) + 6);
		strcpy(response, FILE_FOUND);
		strcat(response, DELIMITER);
		strcat(response, bufferData);
		if (sendto(*args->socket, response, strlen(response), 0, (struct sockaddr*) &args->client, *args->slen) == -1)
		{
			printf("Erro ao enviar arquivo solicitado");
		}
	}
	else {
		message = "Arquivo não existe";
		if (sendto(*args->socket, message, strlen(message), 0, (struct sockaddr*) &args->client, *args->slen) == -1)
		{
			printf("Erro");
		}
	}	
}

int main (void)
{
	createStorageIfNotExists();
	struct sockaddr_in si_me, si_other;
	int s, i, slen = sizeof (si_other), recv_len;
	char buf[BUFLEN];		//create a UDP socket 
  	if ((s = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
      die ("socket");
    }

	memset ((char *) &si_me, 0, sizeof (si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons (PORT);
	si_me.sin_addr.s_addr = htonl (INADDR_ANY);	//bind socket to port 

	if (bind (s, (struct sockaddr *) &si_me, sizeof (si_me)) == -1)
    {
      die ("bind");
    }

	listen(s , 10);
	
	while (1)
	{
		printf ("Waiting for data...\n");
		fflush (stdout);
		setbuf(stdin, NULL);
		if ((recv_len = recvfrom (s, buf, BUFLEN, 0, (struct sockaddr *) &si_other,&slen)) == -1){
			die ("recvfrom()");
		}

		fflush (stdout);
		setbuf(stdin, NULL);

		struct requestData requestArgs;
		requestArgs.client = si_other;
		requestArgs.socket = &s;
		requestArgs.slen = &slen;
		buf[slen] = '\0'; //define o fim da string
		requestArgs.buffer = buf;

		pthread_t thread;
		pthread_create(&thread, NULL, handleRequest, (void *) &requestArgs);
		pthread_join(thread, NULL);
	}

	pthread_exit(NULL);

	// pclose(s);

  return 0;
}

