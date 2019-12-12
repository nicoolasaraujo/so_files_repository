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
}request;

void sendFileToServer(int socket){
	char * data = "send file to server"; //deverá ser os bytes do arquivo selecionado
	char * buffer = malloc(strlen(data) + 7); //Tamanho máximo verbo da requisião + delimitador
	char * response;
	strcpy(buffer, DELIMITER);
	strcat(buffer, data);
	printf("%s", buffer);
	send(socket, buffer, strlen(buffer), 0 );
	int size = read( socket , response, 1024);

	printf("%s", response);
	
}

void getFileFromServer(){
	char * buffer = "send file to server";
	// send(socket, buffer, strlen(buffersend), 0 );
}

int main(int argc, char const *argv[]) 
{
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	char *hello = "Hello from client";
	char buffer[1024] = {0};
	char *fileName;
	int userInput;

	//cria um socket udp para falar com o servidor
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	}

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	 
	// força conexão no localhost porta 8080 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	}

	// tenta abrir conexão com o servidor
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1;
	}
	
	do {
		printf("O que deseja fazer?\n0.Buscar um arquivo\n1.Enviar um arquivo\n2.sair");
		scanf("%d", &userInput);
		if(userInput == 2)
			break;

		if(userInput == 1){
			printf("Digite o nome do arquivo que deseja baixar.");
			scanf("%s", fileName);
			//TODO: Implementar rotinar que envia o nome de um arquivo para o servidor
			getFileFromServer();

		} else if(userInput == 2) {
			printf("Digite o nome do arquivo de que deseja enviar.");
			scanf("%s", fileName);
			//TODO: Implementar rotina que abre o arquivo desejado pelo usuário, e enviar os bytes
			sendFileToServer(sock);

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
