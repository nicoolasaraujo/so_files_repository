#include <stdio.h> 
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 8080
#define DELIMITER "#@#" //string que será utilizada para identificar o tipo da requsição GET: buscar um arquivo; POST: Enviar um arquivo

enum requestType {
	getFile = 1,
	sendFile = 2
} request;

void sendFileToServer(int *socket, struct sockaddr *addr, char * fileName){
	char * data = "send file to server"; //deverá ser os bytes do arquivo selecionado
	char * buffer = malloc(strlen(data) + 7); //Tamanho máximo verbo da requisião + delimitador
	char * response;
	strcpy(buffer, "POST");
	strcat(buffer, DELIMITER);
	strcat(buffer, data);
	printf("%s", buffer);
	sendto(*socket, buffer, strlen(buffer) , 0 , addr, sizeof(*addr));
	// int size = read( socket , response, 1024);K

	// printf("%s", response);
	
}

void getFileFromServer(int *socket, struct sockaddr *addr, char * fileName, int * slen){

	char * buffer = malloc(strlen(fileName) + 7); //Tamanho máximo verbo da requisião + delimitador
	char * response;
	strcpy(buffer, "GET");
	strcat(buffer, DELIMITER);
	strcat(buffer, fileName);
	buffer[strlen(buffer)] = '\0';
	printf("%s\n", buffer);
	printf("%s\n", fileName);
	sendto(*socket, buffer, strlen(buffer) , 0 , addr, sizeof(*addr));
	char buf[1024];
	// while (1)
	// {
		if (recvfrom(*socket, buf, 1024, 0, addr, slen) == -1)
		{
			printf("erro\n");
		}
		puts(buf);


	// }
	
	
}

int main(int argc, char const *argv[]) 
{
	struct sockaddr_in serv_addr; 
	int sock = 0, slen = sizeof(serv_addr); 
	char *hello = "Hello from client";
	char buffer[1024] = {0};
	char *fileName;
	int userInput;

	//cria um socket udp para falar com o servidor
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	}
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	 
	// força conexão no localhost porta 8080 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	}

	do {
		printf("O que deseja fazer?\n0.Buscar um arquivo\n1.Enviar um arquivo\n2.sair\n");
		scanf("%d", &userInput);
		fflush(stdout);
		setbuf(stdin, NULL);
		
		if(userInput == 2)
			break;

		if(userInput == 0){
			printf("Digite o nome do arquivo que deseja baixar\n");
			scanf("%s", fileName);
			//TODO: Implementar rotinar que envia o nome de um arquivo para o servidor
			getFileFromServer(&sock, (struct sockaddr *) &serv_addr, fileName, &slen);

			// if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &amp;si_other, &amp;slen) == -1)
			// {
			// 	die(&quot;recvfrom()&quot;);
			// }

		} else if(userInput == 1) {
			printf("Digite o nome do arquivo de que deseja enviar\n");
			scanf("%s", fileName);
			//TODO: Implementar rotina que abre o arquivo desejado pelo usuário, e enviar os bytes
			sendFileToServer(&sock, (struct sockaddr *) &serv_addr, fileName);

		} else {
			printf("Entrada inválida");
		}
	} while(userInput != 2);

	// printf("%s", buffer);
	// printf("Hello message sent\n"); 
	// valread = read( sock , buffer, 1024); 
	// printf("%s\n",buffer );
	return 0; 
}

// int sendFile(char *fileName, int requestType, int socket){
// 	FILE *f = fopen("storageFiles/TST.txt", "rb");
//     fseek(f, 0, SEEK_END);
//     long fsize = ftell(f);
//     fseek(f, 0, SEEK_SET);
//     char *buffersend = malloc(fsize + 1);
//     fread(buffersend, 1, fsize, f);
//     fclose(f);

// 	printf("Send Buffer: %s", buffersend);
// 	send(socket, buffersend, strlen(buffersend), 0 );

// }
