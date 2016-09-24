#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <sys/un.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#define MAXLINE 1024
#define SOCKET_ADDRESS "/home/pi/socket.dat"

struct timeval start, end;

int main(int argc, char **argv)
{

	int client_len;
	int client_sockfd;

	FILE *fp_in;
	char buf_in[MAXLINE];
	char buf_get[MAXLINE];

	struct sockaddr_un clientaddr;
	
	/*
	if (argc != 2)
	{       
		printf("Usage : %s [file_name]\n", argv[0]);
		printf("예    : %s /tmp/mysocket\n", argv[0]);
		exit(0);
	}       
	*/

//	gettimeofday(&start,NULL);
	int i=0;
	float sum=0;
	float avg=0;
	for(i=0;i<1000;i++){
	gettimeofday(&start,NULL);	
	
	client_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (client_sockfd == -1)
	{
		perror("error : ");
		exit(0);
	}
	bzero(&clientaddr, sizeof(clientaddr));
	clientaddr.sun_family = AF_UNIX;
	strcpy(clientaddr.sun_path, SOCKET_ADDRESS);
	client_len = sizeof(clientaddr);



	if (connect(client_sockfd, (struct sockaddr *)&clientaddr, client_len) < 0)
	{
		perror("Connect error: ");
		exit(0);
	}
//	while(1){
		memset(buf_in, 0x00, MAXLINE);
		memset(buf_get, 0x00, MAXLINE);
//		printf("> ");
//		fgets(buf_in, MAXLINE, stdin);
		strcpy(buf_in, "test string\n");
		write(client_sockfd, buf_in, strlen(buf_in));
		read(client_sockfd, buf_get, MAXLINE); 
//		printf("-> %s", buf_get);
		close(client_sockfd);
	
		gettimeofday(&end, NULL);
//		printf(" %ld\n", (((end.tv_sec * 1000000) + end.tv_usec) - ((start.tv_sec * 1000000) + start.tv_usec)));
		sum += (((end.tv_sec * 1000000) + end.tv_usec) - ((start.tv_sec * 1000000) + start.tv_usec));
	}
//	}
	avg = sum / 1000;
	printf("%f\n", avg);
	exit(0);


}



