/* Simple udp server */
#include <stdio.h>		//printf
#include <string.h>		//memset
#include <stdlib.h>		//exit(0);
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#define BUFLEN 1024		//Max length of buffer
#define PORT 8080		//The port on which to listen for incoming data

void die (char *s)
{
	perror (s);
	exit (1);
}

struct requestData {
	char *buffer;
	struct sockaddr_in client;
};

void * handleRequest(void *ptr){
	struct requestData * args = (struct requestData *)ptr;
	printf("%s", inet_ntoa(args->client.sin_addr));
	printf("Thread Data: %s\n", args->buffer);

}

int main (void)
{
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

	while (1)
	{
		printf ("Waiting for data...");
		fflush (stdout);
		if ((recv_len = recvfrom (s, buf, BUFLEN, 0, (struct sockaddr *) &si_other,&slen)) == -1){
			die ("recvfrom()");
		}

		fflush (stdout);

		printf ("Received packet from %s:%d\n", inet_ntoa (si_other.sin_addr),ntohs (si_other.sin_port));

		struct requestData requestArgs;
		requestArgs.client = si_other;
		requestArgs.buffer = buf;

		pthread_t thread;
		pthread_create(&thread, NULL, handleRequest, (void *) &requestArgs);
		// pthread_join(thread, NULL);

		// printf ("Data: %s\n", buf);	
		if (sendto (s, buf, recv_len, 0, (struct sockaddr *) &si_other, slen) == -1)
		{
			die ("sendto()");
		}

	} 

	pthread_exit(NULL);

	// pclose(s);

  return 0;
}
